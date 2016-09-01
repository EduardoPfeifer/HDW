/*
 * File:   main.c
 * Author: bruno
 *
 * Created on September 1, 2016, 4:44 PM
 */


#include <xc.h>

void main(void) {
    INTCONbits.GIE = 0;
    while(1) {
        asm("nop");
    }
    return;
}
