#include <pic16lf1786.h>

#include "oscillator.h"

void oscillator_init_internal() {
    OSCCONbits.SCS = 0b11; // Internal
    OSCCONbits.SPLLEN = ( OSCILLATOR & 0b00010000 ) >> 4;
    OSCCONbits.IRCF = ( OSCILLATOR & 0b00001111 );   
}

uint32_t oscillator_get_frequency() {
    uint32_t fosc;
    switch( OSCCONbits.IRCF ) {
        case 0b1111: fosc = 16000000ul; break; // HF, *32MHz
        case 0b1110: fosc =  8000000ul; break; // HF, *32MHz
        case 0b1101: fosc =  4000000ul; break; // HF
        case 0b1100: fosc =  2000000ul; break; // HF
        case 0b1011: fosc =  1000000ul; break; // HF
        case 0b1010: fosc =   500000ul; break; // HF
        case 0b1001: fosc =   250000ul; break; // HF
        case 0b1000: fosc =   125000ul; break; // HF
        case 0b0111: fosc =   500000ul; break; // MF
        case 0b0110: fosc =   250000ul; break; // MF
        case 0b0101: fosc =   125000ul; break; // MF
        case 0b0100: fosc =    62500ul; break; // MF
        case 0b0011: fosc =    31250ul; break; // HF
        case 0b0010: fosc =    31250ul; break; // MF
        case 0b0001: fosc =    31000ul; break; // LF
        case 0b0000: fosc =    31000ul; break; // LF
        default:
            fosc = 0ul;
    }
    // Se for 16MHz ou 8MHz, tem a possibilidade de estar ativado o PLLx4, que dai manda pra 32MHz o oscilador.
    if( OSCCONbits.SPLLEN && ( fosc == 16000000ul || fosc == 8000000ul ) ) {
        fosc = 32000000ul;
    }
    return fosc;
}