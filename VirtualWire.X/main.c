#include <htc.h>
#include "virtualwire.h"
#include <string.h>

/*CONFIGs*/
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 4000000

void interrupt global_isr(void) {
    if(T0IF) {
		vw_isr_tmr0();
        RA0 = !RA0;
    }
}

#ifdef XPRJ_TX
void envia( const char* text ) {
    PORTBbits.RB4 = text[0] == '1' ? 1 : 0;
    vw_send( text, strlen(text) );
    //__delay_ms(2000);
}
#endif

void main(void) {	
    TRISA0 = 0;
    RA0 = 1;
    
    TRISB4 = 0;
    RB4 = 0;

	vw_setup(600);

#ifdef XPRJ_TX    
	while(1) {
        envia("0");
        envia("1");
	}
#endif    
#ifdef XPRJ_RX
    uint8_t mesg[VW_MAX_MESSAGE_LEN];
    uint8_t lenght;
    
    vw_rx_start();
    while(1) {
        if( vw_have_message() ) {
            lenght = VW_MAX_MESSAGE_LEN;

            if( vw_recv( mesg, &lenght ) ) {
                PORTBbits.RB4 = mesg[0] == '1' ? 1 : 0;
            }
        }        
    }
#endif
}