#include <pic16lf1786.h>
#include <xc.h>

#include "analog.h"

callback_isr_t _analog_callback_isr;

void analog_start(
    analog_voltage_reference_positive_t voltage_reference_positive,
    analog_voltage_reference_negative_t voltage_reference_negative,
    analog_conversion_clock_t conversion_clock,
    analog_result_format_t analog_result_format,
    analog_result_mode_bit_t analog_result_mode_bit,
    analog_channel_select_t channel_select
) {
    _analog_callback_isr = NULL;    
    
/* 1. Configure Port:
	- Disable pin output driver (Refer to the TRIS register)
	- Configure pin as analog (Refer to the ANSEL register)
*/
    TRISAbits.TRISA0 = INPUT;
    ANSELAbits.ANSA0 = HIGH;
    
/* 2. Configure the ADC module:
	- Select ADC conversion clock
	- Configure voltage reference
	- Select ADC input channel
	- Turn on ADC module
*/    
    ADCON1bits.ADCS = conversion_clock;
    
    ADCON1bits.ADPREF = voltage_reference_positive;
    ADCON1bits.ADNREF = voltage_reference_negative;
    
    ADCON1bits.ADFM = analog_result_format;
    
    ADCON0bits.ADRMD = analog_result_mode_bit;
    
    analog_channel_select( channel_select, FALSE );        
    
    ADCON0bits.GO = LOW;
    
    ADCON0bits.ADON = HIGH;    
}

void analog_channel_select( analog_channel_select_t channel_select, uint8_t skip_delay ) {
    ADCON0bits.CHS = channel_select;
    ADCON2bits.CHSN = 0b1111; // Single Ended ADC
    
    // Tecnicamente, o delay deveria seguir a tabela 17-1, mas por hora, vai ser igual pra todos.
    if( skip_delay ) {
    //TODO delay_us(6);
        NOP();
    }
}

uint16_t analog_read_interrupt_get_value() {
    // Verifica se tá no modo de Sinal + Magnitude. Neste modo, 10 á 12 bits são a magnitude, 1 bit é o sinal e o resto é 0. Ex: mmmm mmmm mmmm 000s
    if( ADCON1bits.ADFM == analog_result_format_sign_magnitude ) {
        // Verifica se estava usando 12 ou 10 bits, e determina o shift que irá fazer com base nisso. Vide tabela 17-2.
        uint8_t shift = ADCON0bits.ADRMD == analog_result_mode_bit_12 ? 16-12 : 16-10; // 4 ou 6, respectivamente.
        // Recupera o resultado da conversão, junto com dados adicionais, como o sinal. Ex: -2355 = 1001 0011 0011 0001 -> bit 0 é o sinal.
        uint16_t result = ADRES;
        // Separa o sinal do resultado todo. Ex: -2355 = 1001 0011 0011 0001 -> 1
        uint8_t sign = result & 0x0001;
        // Desloca o resultado para direita, para ficar no valor que foi lido mesmo. Ex: 1001 0011 0011 0001 >> 4 -> 0000 1001 0011 0011
        result = result >> shift;
        // Se tinha o sinal de negativo, deixa o resultado final como negativo.
        if( sign ) {
            result = -result;
        }
        // finalmente, retorna o resultado.
        return result;
    // Tá no modo de Complemento de 2. Se o bit mais significativo for 1, tem que inverter todos os bits e somar +1 para tirar a resposta correta.
    } else {
        // Recupera o resultado da conversão. O formato é: xxxx mmmm mmmm mmmm ou 
        // xxxx xxmm mmmm mmmm, onde se x for 1, é porque o valor é negativo e está em complemento de 2.
        uint16_t result = ADRES;
        if( result & 0x8000 ) {
            // Complemento de 1
            result = ~result;
            // Complemento de 2
            result++;
            // Deixa o valor negativo.
            result = -result;
        }
        return result;
    }
}

uint16_t analog_read_lock() {
    // Inicia a conversão
    ADCON0bits.GO = HIGH;
    // Fica preso nessa linha até a conversão terminar
    while( ADCON0bits.GO ) { NOP(); }
    // Retorna o valor convertido.
    return analog_read_interrupt_get_value();
}

void analog_read_interrupt( callback_isr_t analog_callback_done ) {
    // seta a função que será chamada no final da transmissão
    _analog_callback_isr = analog_callback_done;
    // Ativa as interrupções de periféricos externos.
    INTCONbits.PEIE = HIGH;
    // Garante que a flag de interrupção está em low.
    PIR1bits.ADIF = LOW;
    // Ativa o uso de interrupções para transmissão
    PIE1bits.ADIE = HIGH;
    // Inicia a conversão
    ADCON0bits.GO = HIGH;
}

void analog_read_isr() {
    // Primeiramente, limpa a flag de interrupção.
    PIR1bits.ADIF = LOW;
    // Chama a função callback (que deve ler o valor usando a função analog_read_get_value)
    if( _analog_callback_isr ) {
        _analog_callback_isr();
        _analog_callback_isr = NULL;
    }
    // Tambem desabilita as interrupções para ADC, já que ele só ativa quando for mandar fazer leitura.
    PIE1bits.ADIE = LOW;
}