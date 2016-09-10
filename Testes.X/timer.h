/* 
 * File:   timer.h
 * Author: bruno
 *
 * Created on September 8, 2016, 4:54 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include "types.h"

typedef enum {
    TIMER0_MODE_TIMER = LOW,
    TIMER0_MODE_COUNTER = HIGH    
} timer0_mode_t;

typedef enum {    
    PRESCALAR_USE_NO  = LOW,
    PRESCALAR_USE_YES = HIGH
} prescalar_use_t;

typedef enum { 
    PRESCALAR_RATE_2   = 0b000,
    PRESCALAR_RATE_4   = 0b001,
    PRESCALAR_RATE_8   = 0b010,
    PRESCALAR_RATE_16  = 0b011,
    PRESCALAR_RATE_32  = 0b100,
    PRESCALAR_RATE_64  = 0b001,
    PRESCALAR_RATE_128 = 0b010,
    PRESCALAR_RATE_256 = 0b111
} prescalar_rate_t;

/**
 * 
 * @param mode Modo de execução (Timer/Counter)
 * @param prescalar_use Usa prescalar de hardware
 * @param prescalar_rate Só é relevante se prescalar_use for HIGH.
 * @param timer0_isr rotina executada quando der uma interrupção.
 */
void timer0_start( 
    timer0_mode_t mode, 
    prescalar_use_t prescalar_use, 
    prescalar_rate_t prescalar_rate, 
    callback_isr_t timer0_isr 
);

/**
 * Invoca a rotina associada a interrupção do timer.
 */
void timer0_isr();

#endif	/* TIMER_H */

