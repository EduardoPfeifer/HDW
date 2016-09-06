/*
 * File:   main.c
 * Author: bruno
 *
 * Created on September 1, 2016, 4:44 PM
 */


#include <xc.h>



void setup_timer();
void setup_adc();
void setup_interrupts();
void interrupt ISR(void);


void main(void) {
    //// Clock 31Khz
    OSCCONbits.IRCF = 0;
    //// @ <SETUP> @
    /// Interrupcao global ativadas e com prioridade, enable interrupt tmr1

    
    
    while(1) {
        asm("nop");
    }
    return;
}

void setup_interrupts()
{
    INTCONbits.GIE = 0;/// global interrupt
    INTCONbits.PEIE = 1; /// priority interrupt
    PIE1bits.TMR1IE;   
    INTCONbits.INTE = 1;
    OPTION_REGbits.INTEDG = 1;
    
    
}

void setup_timer()
{
    /// Timer1
    T1CONbits.TMR1ON = 1;
    T1CONbits.TMR1CS = 1; // clock source frequencia do sistema.
    T1CONbits.T1OSCEN = 1;// dont care se usar fosc.
    T1GCONbits.TMR1GE = 0;// disable gate circuit
    T1GCONbits.T1GPOL = 0;
    T1CONbits.nT1SYNC = 1;
    
}

void setup_adc()
{
    TRISA0 = 1;
    ADCON0bits.ADON = 1;

}


void interrupt ISR(void)
{
//// tratar interrupcao 

}