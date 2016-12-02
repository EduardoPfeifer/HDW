/* 
 * File:   usart.h
 * Author: bruno
 *
 * Created on September 19, 2016, 7:22 PM
 */

#ifndef ANALOG_H
#define	ANALOG_H

#include "types.h"

typedef enum analog_voltage_reference_positive_e {
    analog_voltage_reference_positive_vdd = 0b00,
    analog_voltage_reference_positive_vrefp = 0b01,
    analog_voltage_reference_positive_fvr = 0b11    
} analog_voltage_reference_positive_t;

typedef enum analog_voltage_reference_negative_e {
    analog_voltage_reference_negative_vss = 0b0,
    analog_voltage_reference_negative_vrefm = 0b1
} analog_voltage_reference_negative_t;

typedef enum analog_conversion_clock_e {
    analog_conversion_clock_fosc_2  = 0b000,
    analog_conversion_clock_fosc_4  = 0b100,
    analog_conversion_clock_fosc_8  = 0b001,
    analog_conversion_clock_fosc_16 = 0b101,
    analog_conversion_clock_fosc_32 = 0b010,
    analog_conversion_clock_fosc_64 = 0b110,
    analog_conversion_clock_frc     = 0b011
} analog_conversion_clock_t;

/**
 * Indica o formato do resultado da conversão.
 */
typedef enum analog_result_format_e {
    /**Sinal + Magnitude*/
    analog_result_format_sign_magnitude = 0b0,    
    /**Complemento de 2*/
    analog_result_format_twos_compliment = 0b1
} analog_result_format_t;

/**
 * Indica quantos bits serão utilizados na conversão do sinal.
 */
typedef enum analog_result_mode_bit_e {
    /**Converte o sinal com 12 bits*/
    analog_result_mode_bit_12 = 0b0,
    /**Converte o sinal com 10 bits*/
    analog_result_mode_bit_10 = 0b1
} analog_result_mode_bit_t;

/**
 * Indica qual canal deverá ser selecionado.
 * @todo Por hora, apenas o canal 0 foi implementado. Ver a necessidade de utilizar os outros.
 */
typedef enum analog_channel_select_e {
    analog_channel_select_0 = 0b00000
} analog_channel_select_t;

/**
 * Inicializa e ativa o uso do conversor Analógico/Digital.
 * @param voltage_reference_positive
 * @param voltage_reference_negative
 * @param conversion_clock
 * @param analog_result_format
 * @param analog_result_mode_bit
 * @param channel_select
 */
void analog_start(
    analog_voltage_reference_positive_t voltage_reference_positive,
    analog_voltage_reference_negative_t voltage_reference_negative,
    analog_conversion_clock_t conversion_clock,
    analog_result_format_t analog_result_format,
    analog_result_mode_bit_t analog_result_mode_bit,
    analog_channel_select_t channel_select
);

/**
 * Faz a seleção do canal que será usado.
 * @param channel_select O canal que será usado
 * @param skip_delay Quando verdadeiro, ignora o delay obrigatório, necessário para troca do canal.
 */
void analog_channel_select( analog_channel_select_t channel_select, uint8_t skip_delay );

/**
 * Faz uma conversão analógico digital em modo de espera.
 * @return O valor convertido.
 */
uint16_t analog_read_lock();

/**
 * Faz a conversáo de analógico para digital, utilizando interrupção.
 * @param analog_callback_done Rotina que será executada quando a interrupção ocorrer.
 * @remark Para recuperar o valor convertido, utilize a função analog_read_interrupt_get_value
 */
void analog_read_interrupt( callback_isr_t analog_callback_done );

/**
 * Recupera o valor da conversão analógico-digital, já convertido para uso normal.
 * @return O valor convertido
 */
uint16_t analog_read_interrupt_get_value();

/**
 * Função chamada na rotina de interrupção.
 * @remark Está função só deve ser chamada na interrupção. O programador final não precisa invocar ela.
 */
void analog_read_isr();

#endif//ANALOG_H

