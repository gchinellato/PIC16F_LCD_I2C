#include <xc.h>
#include "serial.h"

void USART_Init(void){
    TRISCbits.TRISC7 = 0x01; // RX input
    TRISCbits.TRISC6 = 0x00; // TX output
    
    //Configure USART
    TXSTA = 0b00100100;        // 8-bit, tx-enb, asynch, BRGH=1
    SPBRG = 100;                // 9600 baud with 20MHz clock
    RCSTA = 0b10010000;        // SPEN=1, CREN=1, addr-det disabled.
}

// this send character routine is called by all the C library put/print routines
void putch(unsigned char chr)
{
    while (TXIF == 0);   // wait until TXREG is not busy
    TXREG = chr;        // send character
}
