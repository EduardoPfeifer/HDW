/*
 * File:   main.c
 * Author: bruno
 *
 * Created on September 1, 2016, 5:15 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#include <pic16lf1786.h>

#include "types.h"
#include "timer.h"
#include "usart.h"
#include "analog.h"
// Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config FOSC = INTOSC
// Watchdog Timer Enable (WDT disabled)
#pragma config WDTE = OFF

/**
 * Interrupt Service Routine (ISR) para tratamento de interrupções geral.
 * 
 * INTCONbits.GIE vira LOW quando entra aqui, e HIGH quando sai, automaticamente.
 */
void interrupt isr_general() {            
    // ISR Timer0
    if( INTCONbits.TMR0IE == HIGH && INTCONbits.TMR0IF == HIGH ) {        
        timer0_isr();
    }
    if( PIE1bits.TXIE == HIGH && PIR1bits.TXIF == HIGH ) {
        usart_transmite_interrupt_isr();
    }
    if( PIE1bits.RCIE == HIGH && PIR1bits.RCIF == HIGH ) {
        usart_receive_interrupt_isr();
    }
    if( PIE1bits.ADIE == HIGH && PIR1bits.ADIF == HIGH ) {
        analog_read_isr();
    }
}

void baby_callback() {
    uint16_t valor = analog_read_interrupt_get_value();

    char memoria[64];
    sprintf( memoria, "Oi, eu sou o goku! Meu ki é: %x\r\n", valor );                        
    usart_transmite_lock_write_message( memoria );    
}

void main(void) {
    TRISBbits.TRISB4 = 0;
    PORTBbits.RB4 = 0;    
    
    // timer0_start( TIMER0_MODE_TIMER, PRESCALAR_USE_YES, PRESCALAR_RATE_256, isr_timer0_pisca_led );
    
    usart_start( USART_SYNC_MODE_ASYNCHRONOUS, USART_BAUD_RATE_300 );
    
    analog_start(
        analog_voltage_reference_positive_vdd,
        analog_voltage_reference_negative_vss,
        analog_conversion_clock_frc,
        analog_result_format_sign_magnitude,
        analog_result_mode_bit_12,
        analog_channel_select_0
    );
    
    INTCONbits.GIE = HIGH;    
    
    
    while(1) {
        byte comando = usart_receive_lock_read_byte();
        if( comando == 'a' ) {
            analog_read_interrupt( baby_callback );
            
            /*
            uint16_t valor = analog_read_lock();
            
            char memoria[64];
            sprintf( memoria, "Oi, eu sou o goku! Meu ki é: %x\r\n", valor );                        
            usart_transmite_lock_write_message( memoria );
            */
        }
        SLEEP();
        //NOP();
    }
    
    return;
}
