#include "usart.h"

#include <pic16lf1786.h>
#include "string.h"

/**
 * Fun��o disparda quando a mensagem for totalmente enviada.
 */
callback_isr_t _usart_callback_transmit_done = NULL;

char _tx_buffer[TX_BUFFER_MAX_SIZE];
/***/
char * _tx_iterator = NULL;

byte _tx_done = HIGH;

void usart_start( usart_sync_mode_t usart_sync_mode, uint32_t baud_rate ) {
    // Ativa o envio (Transmit Enable)
    TX1STAbits.TXEN = HIGH;
    // Setting the SPEN bit of the RCSTA register enables the EUSART and automatically configures the TX/CK I/O pin as an output.
    RCSTAbits.SPEN = HIGH;
    // Synchronization Mode
    TX1STAbits.SYNC = usart_sync_mode;
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
    
    // TODO: Implementar o protocolo pra 9 bits, se necess�rio.
    TXSTAbits.TX9 = USART_9BIT_MODE_NOT_IMPLEMENTED;
    
    // Faz o calculo do Baud Rate!
    // Primeiro, determina a frequencia
    uint32_t fosc;
    switch( OSCCONbits.IRCF ) {
        case 0b1111: fosc = 16000000ul; break; // HF, *32MHz
        case 0b1110: fosc =  8000000ul; break; // HF, *32MHz
        case 0b1101: fosc =  4000000ul; break; // HF
        case 0b1100: fosc =  2000000ul; break; // HF
        case 0b1011: fosc =  1000000ul; break; // HF
        case 0b1010: fosc =   500000ul; break; // HF
        case 0b1001: fosc =   250000ul; break; // HF
        case 0b1000: fosc =   125000ul; break; // HF
        case 0b0111: fosc =   500000ul; break; // MF
        case 0b0110: fosc =   250000ul; break; // MF
        case 0b0101: fosc =   125000ul; break; // MF
        case 0b0100: fosc =    62500ul; break; // MF
        case 0b0011: fosc =    31250ul; break; // HF
        case 0b0010: fosc =    31250ul; break; // MF
        case 0b0001: fosc =    31000ul; break; // LF
        case 0b0000: fosc =    31000ul; break; // LF
        default:
            fosc = 0ul;
    }
    // Se for 16MHz ou 8MHz, tem a possibilidade de estar ativado o PLLx4, que dai manda pra 32MHz o oscilador.
    if( OSCCONbits.SPLLEN && ( fosc == 16000000ul || fosc == 8000000ul ) ) {
        fosc = 32000000ul;
    }
    
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
    _tx_iterator = _tx_buffer;
}

void usart_transmite_interrupt_write_message( char * message, callback_isr_t usart_callback_transmit_done ) {
    // Copia a mensagem e prepara o iterador
    usart_transmite_set_message(message);
    // seta a fun��o que ser� chamada no final da transmiss�o
    _usart_callback_transmit_done = usart_callback_transmit_done;
    // Ativa as interrup��es de perif�ricos externos.
    INTCONbits.PEIE = HIGH;
    // Ativa o uso de interrup��es para transmiss�o
    // � para disparar uma interrup��o imediatamente.
    PIE1bits.TXIE = HIGH;    
}

/**
 * @remark Fun��o privada (N�o acessivel de fora - n�o � pra cagar nela).
 * 
 * Fun��o disparada quando a mensagem foi totalmente enviada.
 */
void usart_transmite_interrupt_done() {
    // Se o canal de envio ainda n�o havia sido encerrado E existe uma fun��o de callback...
    if( PIE1bits.TXIE && _usart_callback_transmit_done ) {
        // Faz a chamada da fun��o
        _usart_callback_transmit_done();
    }
    // Desativa interrup��es para transmiss�o
    PIE1bits.TXIE = LOW;
}

void usart_transmite_interrupt_isr() {
    // Envia o proximo caracter, se ele existir.
    if( *_tx_iterator ) {        
        TXREG = *_tx_iterator;        
        _tx_iterator++;
    // Do contr�rio, encerra o envio e dispara o callback.
    } else {
        usart_transmite_interrupt_done();
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
    // Aguarda a flag ir para low ...    
    while( !PIR1bits.TXIF ) {
        asm("nop");
    }
    // ... para ent�o escrever o byte.
	TXREG = data;
}

void usart_transmite_lock_write_message( char * message ) {
    // Habilita o canal de transmissao e prepara o iterador.
    usart_transmite_set_message( message );
    // Enquanto tiver mensagem para escrever...
    while( *_tx_iterator ) {
        // Escreve um byte ...
        usart_transmite_lock_write_byte( *_tx_iterator );
        // ...  e avan�a um byte por vez
        _tx_iterator++;
    }
    // Finalmente, desabilita o canal de transmissao
    PIE1bits.TXIE = LOW;
}