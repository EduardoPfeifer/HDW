#include <VirtualWire.h>

const int PIN_FOO = 13;
const int PIN_TX = 12;
const int PIN_RX = 11;

const int DELAY = 100;
const int BAUD = 300;

char * controller;

void setup() {
    pinMode( PIN_FOO, OUTPUT );
    
    vw_set_ptt_inverted(true);
    vw_set_tx_pin( PIN_TX );
    // speed of data transfer Kbps
    vw_setup( BAUD );
}

void loop() {
// ---- Envia 1 ----
    controller = "1";
    
    vw_send( (uint8_t*)controller, strlen(controller) );
    vw_wait_tx(); // Wait until the whole message is gone
    
    digitalWrite( PIN_FOO, 1 );
    delay( DELAY );
// ---- Envia 0 ----    
    controller = "0";
    
    vw_send( (uint8_t*)controller, strlen(controller) );
    vw_wait_tx(); // Wait until the whole message is gone
    
    digitalWrite( PIN_FOO, 0 );
    delay( DELAY );
}

