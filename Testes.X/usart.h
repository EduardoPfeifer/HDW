/* 
 * File:   usart.h
 * Author: bruno
 *
 * Created on September 8, 2016, 7:22 PM
 */

#ifndef USART_H
#define	USART_H

#include "types.h"

// TODO talvez trocar os buffers internos por ponteiros para buffers externos.
#define TX_BUFFER_MAX_SIZE 61
#define RX_BUFFER_MAX_SIZE 61

typedef enum {
    USART_SYNC_MODE_ASYNCHRONOUS = LOW,
    USART_SYNC_MODE_SYNCHRONOUS  = HIGH
} usart_sync_mode_t;

typedef enum {
    USART_9BIT_MODE_NOT_IMPLEMENTED = LOW
} usart_9bit_mode_t;

// Defini��o dos Baud Rates mais usados.
const uint32_t USART_BAUD_RATE_300    =    300ul;
const uint32_t USART_BAUD_RATE_1200   =   1200ul;
const uint32_t USART_BAUD_RATE_2400   =   2400ul;
const uint32_t USART_BAUD_RATE_9600   =   9600ul;
const uint32_t USART_BAUD_RATE_10417  =  10417ul;
const uint32_t USART_BAUD_RATE_19200  =  19200ul;
const uint32_t USART_BAUD_RATE_57600  =  57600ul;
const uint32_t USART_BAUD_RATE_115200 = 115200ul;

/**
 * Inicia a comunica��o por EUSART.
 * 
 * @param usart_sync_mode Modo de sincroniza��o desejado. Por hora, s� tem Assincrono.
 * @param baud_rate Velocidade da transferencia. Existem constantes definidas com os valores mais comuns.
 * @todo Implementar modo sincrono!
 */
void usart_start( usart_sync_mode_t usart_sync_mode, uint32_t baud_rate );

/**
 * Transmite uma mensagem, utilizando interrup��es.
 * @param message String que ser� enviada.
 * @param usart_callback_transmit_done Fun��o que ser� chamada ap�s o ultimo byte ser transmitido.
 */
void usart_transmite_interrupt_write_message( char * message, callback_isr_t usart_callback_transmit_done );
/**
 * @remark N�o precisa chamar esta fun��o. Ela � chamada automaticamente pela fun��o tratadora de interrup��es.
 * Fun��o que trata a interrup��o de envio de mensagem.
 * @todo Transformar em uma macro depois.
 */
void usart_transmite_interrupt_isr();

/**
 * Transmite uma string (mas trava a tarefa).
 * @param message String que ser� transmitida.
 */
void usart_transmite_lock_write_message( char * message );

/**
 * Recebe um byte (mas trava a tarefa).
 * @return O valor recebido do RX.
 */
byte usart_receive_lock_read_byte();

/**
 * Recebe uma string inteira (mas trava a tarefa). A string deve terminar em '\r'.
 * @param buffer O buffer onde a mensagem ser� salva.
 * @param size O tamanho do buffer.
 * @return A quantidade de caracteres recebida.
 */
uint8_t usart_receive_lock_read_message( char * buffer, uint8_t size );

/**
 * Recebe uma string inteira, utilizando interrup��es. A string deve terminar em '\r'.
 * @param usart_callback_receive_done fun��o que ser� chamada quando a mensagem for totalmente recebida.
 */
void usart_receive_interrupt_read_message( callback_isr_t usart_callback_receive_done );
/**
 * @remark N�o precisa chamar esta fun��o. Ela � chamada automaticamente pela fun��o tratadora de interrup��es.
 * Fun��o que trata a interrup��o de recep��o de mensagem.
 * @todo Transformar em uma macro depois.
 */
void usart_receive_interrupt_isr();

/**
 * Recupera o ponteiro para a mensagem recebida ao final da interrup��o.
 */
char* usart_receive_interrupt_get_message();

#endif	/* USART_H */

