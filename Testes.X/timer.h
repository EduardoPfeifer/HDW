/* 
 * File:   timer.h
 * Author: bruno
 *
 * Created on September 8, 2016, 4:54 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include "types.h"

/* timer 0*/

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

/*timer 1*/

typedef enum {
    TIMER1_MODE_TIMER = LOW,
    TIMER1_MODE_COUNTER = HIGH    
} timer1_mode_t;

typedef enum{
    TIMER1_SOURCE_LFINTOSC = 0b11,
    TIMER1_SOURCE_SYSCLOCK = 0b01,
    TIMER1_SOURCE_SYSLOCK4 = 0b00,
    TIMER1_SOURCE_EXTERNAL = 0b10
} source_t1;

typedef enum { 
    PRESCALART1_RATE_1 = 0b00,
    PRESCALART1_RATE_2 = 0b01,
    PRESCALART1_RATE_4 = 0b10,
    PRESCALART1_RATE_8 = 0b11

} prescalar_rate_t1;

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
/*
 * Liga o maravilhoso timer 1
 */
void timer1_start(
    timer1_mode_t mode,
    source_t1 source,
    prescalar_rate_t1 prescaler,
    callback_isr_t timer1_isr

);

/**
 * Invoca a rotina associada a interrupção do timer.
 */
void timer0_isr();
void timer1_isr();

#endif	/* TIMER_H */

