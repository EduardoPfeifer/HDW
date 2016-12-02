/*
 * File:   main.c
 * Author: bruno
 *
 * Created on September 1, 2016, 5:15 PM
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#include <pic16lf1786.h>

#include "oscillator.h"
#include "types.h"
#include "timer.h"
#include "usart.h"
#include "analog.h"

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

void main(void) {
    oscillator_init_internal();
    
    TRISBbits.TRISB4 = 0;
    PORTBbits.RB4 = 0;
        
    usart_start( USART_SYNC_MODE_ASYNCHRONOUS, USART_BAUD_RATE_484 );
    
    INTCONbits.GIE = HIGH;
    
    byte led = 0;
    
    while(1) {
#ifdef XPRJ_RX        
        led = usart_receive_lock_read_byte() == 1;
#endif
#ifdef XPRJ_TX
        usart_transmite_lock_write_byte( led );
        led = !led;
#endif        
        PORTBbits.RB4 = led;
    }
    
    return;
}
