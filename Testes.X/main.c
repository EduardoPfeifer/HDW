/*
 * File:   main.c
 * Author: bruno
 *
 * Created on September 1, 2016, 5:15 PM
 */


#include <xc.h>
#include <stdint.h>

#include <pic16lf1786.h>

#include "timer.h"

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
}

void isr_timer0_pisca_led(void) {
    PORTBbits.RB4 = ~LATBbits.LATB4;
}

void main(void) {
    TRISBbits.TRISB4 = 0;
    PORTBbits.RB4 = 0;    
    
    timer0_start( TIMER0_MODE_TIMER, PRESCALAR_USE_YES, PRESCALAR_RATE_256, isr_timer0_pisca_led );
    
    INTCONbits.GIE = HIGH;
    
    while(1) {
        asm("nop");
    }
    
    return;
}
