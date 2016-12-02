/* 
 * File:   oscillator.h
 * Author: bruno
 *
 * Created on November 27, 2016, 10:17 AM
 */

#ifndef OSCILLATOR_H
#define	OSCILLATOR_H

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

#define OSCILLATOR_INTERNAL_FREQUENCY_HF_32MHz		95 // 0b01011111
#define OSCILLATOR_INTERNAL_FREQUENCY_HF_16MHz		79 // 0b01001111
#define OSCILLATOR_INTERNAL_FREQUENCY_HF_8MHz		78 // 0b01001110
#define OSCILLATOR_INTERNAL_FREQUENCY_HF_4MHz		77 // 0b01001101
#define OSCILLATOR_INTERNAL_FREQUENCY_HF_2MHz		76 // 0b01001100
#define OSCILLATOR_INTERNAL_FREQUENCY_HF_1MHz		75 // 0b01001011
#define OSCILLATOR_INTERNAL_FREQUENCY_HF_500kHz		74 // 0b01001010
#define OSCILLATOR_INTERNAL_FREQUENCY_HF_250kHz		73 // 0b01001001
#define OSCILLATOR_INTERNAL_FREQUENCY_HF_125kHz		72 // 0b01001000
#define OSCILLATOR_INTERNAL_FREQUENCY_MF_500kHz		39 // 0b00100111
#define OSCILLATOR_INTERNAL_FREQUENCY_MF_250kHz		38 // 0b00100110
#define OSCILLATOR_INTERNAL_FREQUENCY_MF_125kHz		37 // 0b00100101
#define OSCILLATOR_INTERNAL_FREQUENCY_MF_62500Hz	36 // 0b00100100
#define OSCILLATOR_INTERNAL_FREQUENCY_HF_31250Hz	43 // 0b01000011
#define OSCILLATOR_INTERNAL_FREQUENCY_MF_31250Hz	22 // 0b00100010
#define OSCILLATOR_INTERNAL_FREQUENCY_LF_31000Hz	 0 // 0b00000000

#define OSCILLATOR OSCILLATOR_INTERNAL_FREQUENCY_HF_4MHz

#if OSCILLATOR == OSCILLATOR_INTERNAL_FREQUENCY_HF_4MHz
#define _XTAL_FREQ 4000000
#elif OSCILLATOR == OSCILLATOR_INTERNAL_FREQUENCY_LF_31000Hz
#define _XTAL_FREQ 31000
#endif

#include <xc.h>
#include <stdint.h>

void oscillator_init_internal();

uint32_t oscillator_get_frequency();

#endif	/* OSCILLATOR_H */

