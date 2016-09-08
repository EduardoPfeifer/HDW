/*
 * File:   main.c
 * Author: Pfeifer
 *
 * Created on 18 de Julho de 2016, 00:24
 */

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000
#include <xc.h>
#include "UART.h"
#include <pic16f877a.h>

#define TAM 5
char output[TAM];
char recebido[TAM];
void echo();
void testBuff();
char i, k = 0;

void interrupt Interrupt() {
    if (RCIF) // see if interrupt caused by incoming data
    {
        RCIF = 0; // clear interrupt flag     
        if (FERR || OERR) //Ocorreu erro de framing ou overrun?
        { //Sim...
            CREN = 0x00; //Limpa status de recebimento
            CREN = 0x01;

            #asm retfie;
            #endasm;//Retorna da interrupção
        } //end error test
        
        recebido[i] = RecebeUART(); //leitura de cada caracter

        //Testa se a msg 1 recebida é reconhecida 
        //------------------------------------------------------------------------------//        
        if (recebido[i] == 0x0D) {
            k = 0;
            //Se a string contida na matriz dado = LD1
            if (recebido[0] == 'L' && recebido[1] == 'D' && recebido[2] == '1') {
                EnviaUARTTExtol("LED1 Ligado"); // sends back text 
                PORTDbits.RD1 = 1; //Acende o LED L1
                k = 1;
            }

            if (recebido[0] == 'L' && recebido[1] == 'D' && recebido[2] == '2') {
                EnviaUARTTExtol("LED2 Ligado"); // sends back text 
                PORTDbits.RD2 = 1; //Acende o LED L2
                k = 1;
            }
            //Se a string contida na matriz dado = LD1
            if (recebido[0] == 'D' && recebido[1] == 'D' && recebido[2] == '1') {
                EnviaUARTTExtol("LED1 Desligado"); // sends back text 
                PORTDbits.RD1 = 0; //Acende o LED L1
                k = 1;
            }

            if (recebido[0] == 'D' && recebido[1] == 'D' && recebido[2] == '2') {
                EnviaUARTTExtol("LED2 Desligado"); // sends back text 
                PORTDbits.RD2 = 0; //Acende o LED L2
                k = 1;
            } else if (k == 0) {
                EnviaUARTTExtol("Mensagem nao reconhecida"); // sends back text 
            }
            k = 0;
            i = 0;
        }
        else i++;
    }
}

void main() {

    ADCON0 = 0x00;
    ADCON1 = 0x06;
    CMCON = 0x07;
    TRISB = 0x80;
    PORTB = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;

    T0IE = 0x00;
    T0IF = 0x01;
    GIE = 0x01;
    PEIE = 0x01;

    RCIF = 0x01;
    RCIE = 0x01;

    IniciaUART();
    __delay_ms(100);

    EnviaUARTTExtol("Aguardando recebimento de dados (configurado para 8 Bytes)..."); // sends back text 

    while (1) {

    }
}
