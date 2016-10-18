#include "timer.h"

#include <pic16lf1786.h>

/**
 * Guarda a referencia para a chamada da função de tratamento de interrupção do timer.
 */
callback_isr_t _timer0_callback = NULL;
callback_isr_t _timer1_callback = NULL;


void timer0_start( timer0_mode_t mode, prescalar_use_t prescalar_use, prescalar_rate_t prescalar_rate, callback_isr_t timer0_callback ) {
    OPTION_REGbits.TMR0CS = mode;
    OPTION_REGbits.PSA = prescalar_use ? LOW : HIGH;
    OPTION_REGbits.PS = prescalar_rate;
    
    _timer0_callback = timer0_callback;
    
    INTCONbits.TMR0IF = LOW;
    INTCONbits.TMR0IE = HIGH;
}

void timer0_isr() {
    if( _timer0_callback ) {
        _timer0_callback();
    }
    INTCONbits.TMR0IF = LOW;
}

void timer1_start(timer1_mode_t mode, source_t1 source , prescalar_rate_t1 prescaler,
        callback_isr_t timer1_callback){
    
    T1CONbits.TMR1CS = source;
    T1CONbits.T1CKPS = prescaler;
    _timer1_callback = timer1_callback;
   // Necessario tempo de estabilizacao recomendacao da microchip
    //via software setar TMR1H:TMR1L como FC00h 
    //a proxima interrupcao apos 1024 ciclos ele estara estavel pronto para uso
    PIE1bits.TMR1IE = HIGH;//// liga interrupteta do timer 1
    TMR1IF = LOW; /// limpa flag vai que ne
    T1CONbits.T1OSCEN = HIGH; /// habilitar oscilador interno do timer 1
    T1GCONbits.TMR1GE = mode;/// modo contador ou timer
    T1CONbits.TMR1ON = HIGH; //// ligar o timer 1
}


void timer1_isr() {
    if( _timer1_callback ) {
        _timer1_callback();
    }
    TMR1IF = LOW;
}






