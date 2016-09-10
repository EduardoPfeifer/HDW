#include "timer.h"

#include <pic16lf1786.h>

/**
 * Guarda a referencia para a chamada da função de tratamento de interrupção do timer.
 */
callback_isr_t _timer0_callback = NULL;

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