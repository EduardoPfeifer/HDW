/*
 * File:   main.c
 * Author: bruno
 *
 * Created on September 1, 2016, 5:15 PM
 */


#include <xc.h>
#include <stdint.h>

#include <pic16lf1786.h>

// Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config FOSC = INTOSC
// Watchdog Timer Enable (WDT disabled)
#pragma config WDTE = OFF

void main(void) {
    
    TRISBbits.TRISB4 = 0;
    PORTBbits.RB4 = 0;
    int a;
    while(1) {
        PORTBbits.RB4 = ~LATBbits.LATB4;
        for( uint16_t i = 0; i < 10000; i++ ) {
            
            asm("nop");
            a = i;
        }
    }
    
    return;
}
