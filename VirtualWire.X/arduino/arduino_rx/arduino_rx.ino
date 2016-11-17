#include <VirtualWire.h>

const int PIN_FOO = 13;
const int PIN_TX = 12;
const int PIN_RX = 11;

const int BAUD = 300;

void setup() {
    pinMode( PIN_FOO, OUTPUT );

    vw_set_ptt_inverted(true);
    vw_set_rx_pin( PIN_RX );
    vw_setup( BAUD ); // Bits per sec

    vw_rx_start(); // Start the receiver PLL running
}

void loop() {
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if( vw_get_message( buf, &buflen ) ) {
        if( buf[0] == '1' ) {
            digitalWrite( PIN_FOO, 1 );
        } else if (buf[0] == '0') {
            digitalWrite( PIN_FOO, 0 );
        }
    }
}
