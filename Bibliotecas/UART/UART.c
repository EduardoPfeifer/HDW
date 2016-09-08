/*  File:     UART.c
    Purpose:  To provide UART functionality for PIC uC
*/
#include "UART.h"

void IniciaUART(void)
{
	TRISC6 = 0;   					// TX Pin
	TRISC7 = 1;   					// RX Pin
	
	SPBRG = ((_XTAL_FREQ/16)/BAUDRATE) - 1;
	BRGH  = 1;                   	// Fast baudrate
	SYNC  = 0;						// Asynchronous
	SPEN  = 1;						// Enable serial port pins
	CREN  = 1;						// Enable reception
	SREN  = 0;						// No effect
	TXIE  = 0;						// Disable tx interrupts
	RCIE  = 1;						// Enable rx interrupts
	TX9   = 0;						// 8-bit transmission
	RX9   = 0;						// 8-bit reception
	TXEN  = 0;						// Reset transmitter
	TXEN  = 1;						// Enable the transmitter
}

void EnviaUART(unsigned char Byte)  // Writes a character to the serial port
{
	while(!TXIF);  // wait for previous transmission to finish
	TXREG = Byte;
}

unsigned char RecebeUART(void)   // Reads a character from the serial port
{
	if(FERR || OERR) // If over run error, then reset the receiver
	{
		CREN = 0;
		CREN = 1;
	}
	
	while(!RCIF);  // Wait for transmission to receive	
	return RCREG;
}

void EnviaUARTTExto(const unsigned char* st)
{
	while(*st) EnviaUART(*st++);
}

void EnviaUARTTExtol(const unsigned char* st)
{
	while(*st) EnviaUART(*st++);
    EnviaUART(0x0D);
}

