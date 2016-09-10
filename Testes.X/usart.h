/* 
 * File:   usart.h
 * Author: bruno
 *
 * Created on September 8, 2016, 7:22 PM
 */

#ifndef USART_H
#define	USART_H

#include "types.h"

#define TX_BUFFER_MAX_SIZE 256

typedef enum {
    USART_SYNC_MODE_ASYNCHRONOUS = LOW,
    USART_SYNC_MODE_SYNCHRONOUS  = HIGH
} usart_sync_mode_t;

typedef enum {
    USART_9BIT_MODE_NOT_IMPLEMENTED = LOW
} usart_9bit_mode_t;

// Definição dos Baud Rates mais usados.
const uint32_t USART_BAUD_RATE_300    =    300ul;
const uint32_t USART_BAUD_RATE_1200   =   1200ul;
const uint32_t USART_BAUD_RATE_2400   =   2400ul;
const uint32_t USART_BAUD_RATE_9600   =   9600ul;
const uint32_t USART_BAUD_RATE_10417  =  10417ul;
const uint32_t USART_BAUD_RATE_19200  =  19200ul;
const uint32_t USART_BAUD_RATE_57600  =  57600ul;
const uint32_t USART_BAUD_RATE_115200 = 115200ul;

/**
 * Inicia a comunicação por EUSART.
 * 
 * @param usart_sync_mode Modo de sincronização desejado. Por hora, só tem Assincrono.
 * @param baud_rate Velocidade da transferencia. Existem constantes definidas com os valores mais comuns.
 * @todo Implementar modo sincrono!
 */
void usart_start( usart_sync_mode_t usart_sync_mode, uint32_t baud_rate );

/**
 * Transmite uma mensagem, utilizando interrupções.
 * @param message String que será enviada.
 * @param usart_callback_transmit_done Função que será chamada após o ultimo byte ser transmitido.
 */
void usart_transmite_interrupt_write_message( char * message, callback_isr_t usart_callback_transmit_done );
/**
 * @remark Não precisa chamar esta função. Ela é chamada automaticamente pela função tratadora de interrupções.
 * Função que trata a interrupção.
 * @todo Transformar em uma macro depois.
 */
void usart_transmite_interrupt_isr();

/**
 * Transmite uma string (mas trava a tarefa).
 * @param message String que será transmitida.
 */
void usart_transmite_lock_write_message( char * message );

#endif	/* USART_H */

