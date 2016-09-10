/*
 * File:   main.c
 * Author: bruno
 *
 * Created on September 1, 2016, 5:15 PM
 */


#include <xc.h>
#include <stdint.h>

#include <pic16lf1786.h>

#include "types.h"
#include "timer.h"
#include "usart.h"

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
}

void isr_timer0_pisca_led(void) {
    // PORTBbits.RB4 = ~LATBbits.LATB4;
}

void isr_usart_tx_acende_led(void) {
    PORTBbits.RB4 = ~LATBbits.LATB4;
}

void main(void) {
    TRISBbits.TRISB4 = 0;
    PORTBbits.RB4 = 0;
    
    timer0_start( TIMER0_MODE_TIMER, PRESCALAR_USE_YES, PRESCALAR_RATE_256, isr_timer0_pisca_led );
    
    usart_start( USART_SYNC_MODE_ASYNCHRONOUS, USART_BAUD_RATE_300 );
    
    INTCONbits.GIE = HIGH;
    
    usart_transmite_lock_write_message( "Testezinho" );
    isr_usart_tx_acende_led();
    
    while(1) {
        asm("nop");
    }
    
    return;
}
