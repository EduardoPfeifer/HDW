#include "oscillator.h"

#include "virtualwire.h"

#define LED_TIMER_TRIS TRISAbits.TRISA0
#define LED_TIMER PORTAbits.RA0

#define LED_ENVIO_TRIS TRISBbits.TRISB4
#define LED_ENVIO PORTBbits.RB4

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
    oscillator_init_internal();
    
    LED_TIMER_TRIS = 0;
    LED_ENVIO_TRIS = 0;
    
    LED_TIMER = 1;
    LED_ENVIO = 0;
    
	vw_setup( 600ul );

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