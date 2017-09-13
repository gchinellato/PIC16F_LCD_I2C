#include <xc.h>
#include "i2c.h"
#include "display.h"

void I2C_Master_Init(const unsigned long c)
{
  SSPCON = 0b00101000;            //SSP Module as Master 0x28
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*c))-1; //Setting Clock Speed
  SSPSTAT = 0x80;              // disable slew rate control
  TRISC3 = 1;                   //Setting as input as given in datasheet
  TRISC4 = 1;                   //Setting as input as given in datasheet
}

void I2C_Master_Wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}

void I2C_Master_Start()
{
  I2C_Master_Wait();    
  SEN = 1;             //Initiate start condition
}

void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  RSEN = 1;           //Initiate repeated start condition
}

void I2C_Master_Stop()
{
  I2C_Master_Wait();
  PEN = 1;           //Initiate stop condition 
}

void I2C_Master_Write(unsigned char d)
{
  /*I2C_Master_Wait();
  SSPBUF = d;         //Write data to SSPBUF
  //while(BF);
  //I2C_Master_Wait();*/
  unsigned char ackflag = 1;
  while(ackflag)
  {
    I2C_Master_Wait();
    SSPIF = 0;        // clear MSSP Interrupt Flag
    SSPBUF = d;      // send a char out on bus
    while(!SSPIF);      // goes high after 9th bit time, after ACK bit
        ackflag = 0;
    if(ACKSTAT)
        ackflag = 1; //* if ACTSTAT == 1 (nak) repeat loop
  }
}

void I2C_Ack()
{
   ACKDT = 0;       /* Acknowledge data bit, 0 = ACK */
    ACKEN = 1;       /* Ack data enabled */
	while(ACKEN);    /* wait for ack data to send on bus */ 
}

void I2C_Nak()
{
	ACKDT = 1;       /* Acknowledge data bit, 1 = NAK */
	ACKEN = 1;       /* Ack data enabled */
	while(ACKEN);    /* wait for ack data to send on bus */
}

unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;
  I2C_Master_Wait();
  RCEN = 1;
  I2C_Master_Wait();
  temp = SSPBUF;      //Read data from SSPBUF
  I2C_Master_Wait();
  ACKDT = (a)?0:1;    //Acknowledge bit
  ACKEN = 1;          //Acknowledge sequence
  return temp;
  // unsigned char temp;
/* Reception works if transfer is initiated in read mode */
	//RCEN = 1;        /* Enable data reception */
	//while(!BF);      /* wait for buffer full */
	//temp = SSPBUF;   /* Read serial buffer and store in temp register */
	//I2C_Master_Wait();       /* wait to check any pending transfer */
	//return temp;     /* Return the read data from bus */
}