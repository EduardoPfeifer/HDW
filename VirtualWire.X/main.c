/* 
 * File:   main.c
 * Author: Pedro Augusto Di Francia Rosso
 *
 * Created on 21 de Setembro de 2016, 21:48
 */

/*
 * PORTD -> d2,d3,d4,d5,d6,d7 used on display LCD
 */

#include <stdio.h>
#include <htc.h>
#include "stdint.h"
#include "virtualwire.h"
#include "configbits.h"

#define _XTAL_FREQ 4000000

/*CONFIGs*/
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


//byte *message;
//char sendMessage[] = "OI";

static bank1 uint8_t Rtext[VW_MAX_MESSAGE_LEN];

void interrupt global_isr(void) {
    if (T0IF)
        vw_isr_tmr0();
}

void delay(unsigned int delay) {
    while (delay--);
}

// Send from PIC

void main(void) {
    
    int i = 0;
    char text[2] = {'o', 'i'};
    vw_setup(600);


    while (1) {
            vw_tx_start();
            vw_send((uint8_t *) text, 2);
            vw_wait_tx();
            //debugLedInvert();
            vw_tx_stop();
            __delay_ms(2000);    
       
//        vw_rx_start();
//        uint8_t len = VW_MAX_MESSAGE_LEN;
//
//        while (!vw_recv(Rtext, &len));
//        Lcd_Clear();
//        __delay_ms(100);
//        Lcd_Write_String(Rtext);
//        
//        __delay_ms(2000);
//        initLED = ~initLED;
//        vw_rx_stop();

    }
}



////



//
//void interrupt allISR() {
//    
////    static u_int baseTime = 0;
////    
////    /*Interruption on TMR0*/
////    if (INTCONbits.TMR0IF == 1) {
////        baseTime++;
////        /*Base time is ~1ms*/
////        if (baseTime == 4) {
////            debugLedInvert();
////            baseTime = 0;
////        }
////        INTCONbits.TMR0IF = 0;
////    }
//    
//    debugLedInvert();
//    /*Interruption on UART*/
//    if (PIR1bits.RCIF == 1){
//        message = UARTreceiveString();
//        disableUART();
//        Lcd_Clear();
//        myDelay('s', 1);
//        Lcd_Write_String(message);
//        myDelay('s', 2);
//    }
//}