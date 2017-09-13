/******************************************************************************
 * Hardware I2C single master routines for PIC16F887 to PCF8574A LCD
 * MPLAB XC8 COMPILER.
 *
 * by Hal Albach
 * 
 * created  01/15/2016
 * 
 * modified 01/23/2016
 * 
******************************************************************************/

#ifndef DISPLAY_I2C_H
#define	DISPLAY_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "config.h"
    
typedef unsigned char uchar;
typedef unsigned int  uint;

// Global variables
uchar RS = 0;       // select lcd command register

#define power_up_delay 50
#define nibble          4
#define five            5
#define init_delay_5    5
#define acq_del         5
#define two             2
#define init_delay_2    2
#define line_2          2
#define four_bit        0x20
#define four_more       0x28
#define cursor          0x0C
#define inc_cursor      6
#define null            0x00
#define EN_off          0b11111110
#define ack             0
#define lcd_cmd         0
#define send_stop       0
#define off             0
#define on              1
#define clr_dsp         1
#define lcd_data        1
#define EN_on           0b00000001
#define col_1           1
#define nack            1
#define send_start      1
#define col_13          13
#define all_hi          0xFF
#define RC34            0x18
#define master          0x38
#define bus100          0x39
#define row_1_offset    0x80
#define adc80           0x80
#define adc81           0x81
#define max_buf         10
#define max_samples     30
#define row_2_offset    0xC0
#define upper           0xF0
//#define I2C_adr         0x70
#define I2C_adr         0x40
#define sec_2           2000
#define sec_3           3000
#define SSPCON2_mask    0x1F
#define set_8_bit       0x30


// Prototypes
void display_i2c();
void write_lcd(uchar data);
void i2c_init();
void i2c_start();
void i2c_repStart();
void i2c_stop();
void i2c_out( uchar i2cWriteData );
void lcd_write_byte(uchar byte);
void lcd_init();
void Lcd_Write_String(const uchar *a);
void start(void);
void stop(void);
void Lcd_Clear();
void Lcd_Set_Cursor(uchar a, uchar b);
void test_adc(void);
void strobe(uchar cmd);


// original of altered functions
/****************************************************************************/
/*
void write_lcd(uchar data)
{
  i2c_waitForIdle();
  SSPIF = 0;
  SSPBUF = data;      // send a char out on bus
  while(!SSPIF);      // goes high after 9th bit time (ACK bit))
}
*/


#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_I2C_H */

