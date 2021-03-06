#include "usart.h"

#include <pic16lf1786.h>
#include <pic.h>
#include "oscillator.h"
#include "string.h"

/**Buffer para envio de mensagens*/
char _tx_buffer[TX_BUFFER_MAX_SIZE];
/**Iterador do buffer de envio*/
char* _tx_buffer_iterator = NULL;
/**Fun��o disparda quando a mensagem for totalmente enviada no modo por interrup��o*/
callback_isr_t _tx_isr_done_callback = NULL;

/**Buffer para recebimento de mensagens*/
char _rx_buffer[RX_BUFFER_MAX_SIZE];
/**Iterador do buffer de recebimento*/
char* _rx_buffer_iterator = NULL;
/**Tamanho da mensagem recebida pelo buffer*/
byte _rx_buffer_message_size;
/**Fun��o disparda quando uma mensagem for totalmente recebida (terminada em 'r')*/
callback_isr_t _rx_isr_done_callback = NULL;

void usart_start( usart_sync_mode_t usart_sync_mode, uint32_t baud_rate ) {
    // Ativa o envio (Transmit Enable)
    TX1STAbits.TXEN = HIGH;
    // Setting the SPEN bit of the RCSTA register enables the EUSART and automatically configures the TX/CK I/O pin as an output.
    RCSTAbits.SPEN = HIGH;
    // Synchronization Mode
    TX1STAbits.SYNC = usart_sync_mode;
            
    // Ativa a recep��o (Receive Enable)
    RCSTAbits.CREN = HIGH;
    // Segundo o manual, o pino de entrada deve ser setado manualmente.
    TRISCbits.TRISC7 = INPUT;    
    
    // TODO: Implementar o protocolo pra 9 bits, se necess�rio.
    TXSTAbits.TX9 = USART_9BIT_MODE_NOT_IMPLEMENTED;
    RCSTAbits.RX9 = USART_9BIT_MODE_NOT_IMPLEMENTED;            
        
    // -------------------------------------------------------------------------
    // The default state of this bit is ?0? which selects high true transmit 
    // idle and data bits. Setting the SCKP bit to ?1? will invert the transmit
    // data resulting in low true idle and data bits.
    // -------------------------------------------------------------------------
    // Basicamente, se tiver em modo Assincrono:
    //   SCKP(LOW)  => Faz transmissao com sinal 1
    //   SCKP(HIGH) => Faz transmissao com sinal 0 (invertido)
    if( usart_sync_mode == USART_SYNC_MODE_ASYNCHRONOUS ) {
        BAUDCONbits.SCKP = LOW;
    }
    
    // Faz o calculo do Baud Rate!
    // Primeiro, determina a frequencia
    uint32_t fosc = oscillator_get_frequency();    
    // Fatores que decidem a formula para calculo do Baud Rate
    uint8_t baud_switch = SYNC << 2 | BRG16 << 1 | BRGH << 0;
    // Calibra o valor de SPBRG (X na formula do Baud Rate, p�g. 348) para o Baud Rate desejado.
    switch( baud_switch ) {
        case 0b000: SPBRG = ( fosc / ( 64ul * baud_rate ) ) - 1ul; break;
        case 0b001:
        case 0b010: SPBRG = ( fosc / ( 16ul * baud_rate ) ) - 1ul; break;
        default: // Para as demais combina��es, a formula �:
            SPBRG = ( fosc / ( 4ul * baud_rate ) ) - 1ul; break;
    }
}

/**
 * @remark Fun��o privada (N�o acessivel de fora - n�o � pra cagar nela).
 * 
 * Fun��o que copia a mensagem para um buffer interno E cria um iterador (ponteiro) para percorre ela.
 */
void usart_transmite_set_message( char * message ) {
    // Copia a mensagem para o buffer, para que ela fique disponivel durante todo o processo.
    strcpy( _tx_buffer, message );
    // Inicia o iterador que ira enviar caracter por caracter para o inicio da mensagem.
    _tx_buffer_iterator = _tx_buffer;
}

void usart_transmite_interrupt_write_message( char * message, callback_isr_t usart_callback_transmit_done ) {
    // Copia a mensagem e prepara o iterador
    usart_transmite_set_message(message);
    // seta a fun��o que ser� chamada no final da transmiss�o
    _tx_isr_done_callback = usart_callback_transmit_done;
    // Ativa as interrup��es de perif�ricos externos.
    INTCONbits.PEIE = HIGH;
    // Ativa o uso de interrup��es para transmiss�o
    // � para disparar uma interrup��o imediatamente.
    PIE1bits.TXIE = HIGH;    
}

void usart_transmite_interrupt_isr() {
    // Envia o proximo caracter, se ele existir.
    if( *_tx_buffer_iterator ) {        
        TXREG = *_tx_buffer_iterator;        
        _tx_buffer_iterator++;
    // Do contr�rio, encerra o envio e dispara o callback.
    } else {
        // Se o canal de envio ainda n�o havia sido encerrado E existe uma fun��o de callback...
        if( PIE1bits.TXIE && _tx_isr_done_callback ) {
            // Faz a chamada da fun��o
            _tx_isr_done_callback();
        }
        // Desativa interrup��es para transmiss�o
        PIE1bits.TXIE = LOW;
    }
}

/**
 * @remark Fun��o privada (N�o acessivel de fora - n�o � pra cagar nela).
 *
 * Transmite um byte (mas trava a tarefa).
 *  
 * @param data Byte que ser� transmitido.
 */
void usart_transmite_lock_write_byte( byte data ) {        
    // <TXIF (Interrupt Flag Bit for Transmiting) � controlado por hardware>
    // Aguarda a a transmiss�o anterior encerrar ...
    while( !PIR1bits.TXIF ) {
        NOP();
    }
    // ... para ent�o escrever o byte.
	TXREG = data;
}

void usart_transmite_lock_write_message( char * message ) {
    // Habilita o canal de transmissao e prepara o iterador.
    usart_transmite_set_message( message );
    // Enquanto tiver mensagem para escrever...
    while( *_tx_buffer_iterator ) {
        // Escreve um byte ...
        usart_transmite_lock_write_byte( *_tx_buffer_iterator );
        // ...  e avan�a um byte por vez
        _tx_buffer_iterator++;
    }
    // Finalmente, desabilita o canal de transmissao
    PIE1bits.TXIE = LOW;
}

byte usart_receive_lock_read_byte() {
 	/*// Verifica se ocorreu algum erro (Frame/Overflow)
    if( RCSTAbits.FERR || RCSTAbits.OERR ) {
        //TODO fazer um tratamento de erro
		// Reinicia o receiver, para poder resetar as flags de erro.
        RCSTAbits.CREN = LOW;
		RCSTAbits.CREN = HIGH;        
	}*/
    
    // <RCIF (Interrupt Flag bit for Receiving) � controlador por hardware>
    // Aguarda a flag ir para high (indicando que recebeu 8+ bytes) ...
    while( !PIR1bits.RCIF ) {
        NOP();
    }
    // ... para ent�o fazer a leitura do byte
    return RCREG;
}

uint8_t usart_receive_lock_read_message( char * buffer, uint8_t size ) {
    // Cursor que ir� navegar o buffer
    char* cursor = buffer;
    // Conta a quantidade de caratereres recebida pela mensagem
    uint8_t i;
    // Itera pelo buffer, salvando nele os bytes recebidos
    for( i = 0; i < size; i++ ) {
        // Recebe uma letra do RX
        byte letter = usart_receive_lock_read_byte();
        // Verifica se � final do buffer. Estamos usando o carriage return, mas ideal era ver pelo caracter NULO mesmo...
        if( letter == '\r' ) {
            // Arrumamos a string para final NULO antes de retornar.
            *cursor = '\0';
            break;
        // Se n�o for a letra de termino de string
        } else {
            // salva a letra no buffer e posiciona o cursor na proxima posi��o.
            *cursor = letter;
            cursor++;
        }
    }
    // Retorna a quantidade de caracters lido (incluindo o nulo)
    return i;
}

void usart_receive_interrupt_read_message( callback_isr_t usart_callback_receive_done ) {
    // Inicia as variaveis de interrup��o para RX.
    _rx_buffer_iterator = _rx_buffer;
    _rx_buffer_message_size = 0;
    _rx_isr_done_callback = usart_callback_receive_done;
    // Ativa as interrup��es de perif�ricos externos.
    INTCONbits.PEIE = HIGH;
    // Ativa o uso de interrup��es para transmiss�o
    // � para disparar uma interrup��o imediatamente.
    PIE1bits.RCIE = HIGH;
}

void usart_receive_interrupt_isr() {
    // Recebeu um byte inteiro, e agora armazena ele no buffer.
    *_rx_buffer_iterator = RCREG;    
    // Aumenta o tamanho recebido no buffer.
    _rx_buffer_message_size++;
    
    if( *_rx_buffer_iterator == '\r' ) {
        *_rx_buffer_iterator = '\0';
        // Se o canal de recep��o ainda n�o havia sido encerrado E existe uma fun��o de callback...
        if( PIE1bits.RCIE && _rx_isr_done_callback ) {
            // Faz a chamada da fun��o
            _rx_isr_done_callback();
        }
        // Desativa interrup��es para recep��o
        PIE1bits.RCIE = LOW;
    } else if( _rx_buffer_message_size == (RX_BUFFER_MAX_SIZE-1) ) {
        //TODO deu merda. Veio mais caracteres do que o buffer comportava.
        PIE1bits.RCIE = LOW;
    } else {
        // Move o cursor para a proxima posi��o
        _rx_buffer_iterator++;
    }
}

char* usart_receive_interrupt_get_message() {
    return _rx_buffer;
}