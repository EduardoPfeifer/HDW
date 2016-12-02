// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#include "virtualwire.h"

#define LED_TIMER_TRIS TRISAbits.TRISA0
#define LED_TIMER PORTAbits.RA0

#define LED_RECEBIDO_TRIS TRISBbits.TRISB4
#define LED_RECEBIDO PORTBbits.RB4

void interrupt global_isr(void) {
    if(T0IF) {
		vw_isr_tmr0();
        LED_TIMER = !LED_TIMER;
    }
}

#ifdef XPRJ_TX
void envia( const char* text ) {
    LED_ENVIO = text[0] == '1' ? 1 : 0;
    vw_send( text, 1 );
    __delay_ms(1000);
}
#endif

void main(void) {    
    LED_TIMER_TRIS = 0;
    LED_RECEBIDO_TRIS = 0;
    
    LED_TIMER = 1;
    LED_RECEBIDO = 0;
    
	vw_setup( 600ul );

    uint8_t mesg[VW_MAX_MESSAGE_LEN];
    uint8_t lenght;
    
    vw_rx_start();
    
    while(1) {
        if( vw_have_message() ) {
            lenght = VW_MAX_MESSAGE_LEN;

            if( vw_recv( mesg, &lenght ) ) {
                LED_RECEBIDO = mesg[0] == '1' ? 1 : 0;
            }
        }
    }

}