#include <htc.h>
#include "virtualwire.h"

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


void interrupt global_isr(void) {
	if(T0IF)
		vw_isr_tmr0();
}

void delay(unsigned int delay) {
	while(delay--);
}

void main(void) {	
//    CMCON = 0x07;	// analog comparator disabled
//	VRCON = 0x00;	// voltage reference module disabled
    TRISB4 = 0;
    RB4 = 0;

	vw_setup(300);

	while(1) {
        PORTBbits.RB4 = 0;
        char* text = "0";
		vw_send( text, 1 );
		delay(20000);
        
        PORTBbits.RB4 = 1;
        char* text = "1";
		vw_send( text, 1 );
	}
	
}