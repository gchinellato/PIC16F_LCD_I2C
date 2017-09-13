 /*=========================================================================
  * I2Clcd.X
  * I2C LCD project
  * 
  * MPLAB X v3.20
  * 
  * XC8 COMPILER v1.35
  * 
  * PIC16F887 on a 44-Pin Demo Board

  * by

  * Hal Albach (v Original ;)

  * This program uses the hardware I2C on the 44-Pin Demo Board with a
  * PIC16F887 to write to a PCF8574A controlled 2x32 LCD, no backlight.
  * 8-bit I2C Address for the -A version is 0x70.
  * 
  * Displays some text, then uses the ADC and onboard RP1 to generate values 
  * displayed on LCD.
  * 
  * Cut a small perfboard to just fit the PCF8574A and a 2x7 DIL socket for 
  * the LCD and wired as indicated below.
  *  
  *         Connections:
  * 
  * PCF8574A              PCF8574A
  *   1 Gnd                 16 +5
  *   2 Gnd                 15 SDA to PIC RC4 & 3.3K pullup
  *   3 Gnd                 14 SCL to PIC RC3 & 3.3K pullup
  *   4 P0 to LCD 6 EN      13 n/c
  *   5 P1 to LCD 4 RS      12 P7  to LCD 14 DB7
  *   6 P2 to LCD 5 RW      11 P6  to LCD 13 DB6
  *   7 P3 n/c              10 P5  to LCD 12 DB5
  *   8 Gnd                 09 P4  to LCD 11 DB4
  * 
  
/***************************************************************************/

#include <xc.h>
#include "display-i2c.h"
#include <stdlib.h>         // needed for ITOA() function

/****************************************************************************/

void display_i2c(void)
{
  i2c_init();
  start();        // send start condition and open PCF8574A 
  lcd_init();     // bring the LCD on line
   Lcd_Write_String("Mister Hal sez: Hello, World!!");
  Lcd_Set_Cursor(line_2, col_1);
  Lcd_Write_String("Have a very Happy New Year!");
  __delay_ms(sec_2);
  Lcd_Clear();
  Lcd_Write_String("  Do not look into Laser with");
  Lcd_Set_Cursor(line_2, col_1);
  Lcd_Write_String("      remaining good eye");
  __delay_ms(sec_3);
  Lcd_Clear();
   
  test_adc();
  stop();
    
  while(1);
}

/****************************************************************************/
//                  VERY IMPORTANT for proper I2C function
void i2c_waitForIdle(void)      
{
  while (( SSPCON2 & SSPCON2_mask ) | R_W ); // wait for idle and not writing
}

/****************************************************************************/

void start(void)
{
  i2c_waitForIdle();
  SEN = send_start;
  i2c_waitForIdle();
  SSPBUF = I2C_adr;        // address PCF8574A I2C port expander
}

/****************************************************************************/

void stop(void)
{
  i2c_waitForIdle();
  PEN = send_stop;
}

/****************************************************************************/
// * This function actually initiates I2C write sequence. Also added NACK error
// * checking which will repeat the nacked transmission until an ACK is rec'd.
// * TO DO: Add a time out counter to abort in case I2C bus becomes locked.

void write_lcd(uchar data)
{
  uchar ackflag = nack;
  while(ackflag)
  {
    i2c_waitForIdle();
    SSPIF = off;        // clear MSSP Interrupt Flag
    SSPBUF = data;      // send a char out on bus
    while(!SSPIF);      // goes high after 9th bit time, after ACK bit
    ackflag = ack;
    if(ACKSTAT) ackflag = nack; //* if ACTSTAT == 1 (nak) repeat loop
  }
}

/****************************************************************************/

void lcd_write_byte(uchar byte)
{
  uchar shadow;                   // the world famous "Shadow" register
    
  shadow = (byte & upper);        // EN, RS, RW = 0 0 0
  if(RS) shadow |= 0b00000010;    // skip if cmd, select lcd data register
  write_lcd(shadow);
  strobe(shadow);
// - - - - - - - - - - - - - 
  shadow = (byte << nibble);      // EN, RS, RW = 0 0 0 by left shift fill
  if(RS) shadow |= 0b00000010;    // skip if cmd, select lcd data register
  write_lcd(shadow);
  strobe(shadow);
}

/****************************************************************************/

void Lcd_Write_String(const uchar *sbyte)
{
	int i;
  RS = lcd_data;
	for(i = 0; sbyte[i] != '\0'; i++)
	  lcd_write_byte(sbyte[i]);
  RS = lcd_cmd;
}

/****************************************************************************/

void Lcd_Clear(void)
{
  RS = lcd_cmd;
	lcd_write_byte(clr_dsp);      // clear display, cursor to home position
  __delay_ms(two);
}

/****************************************************************************/
//             Position the cursor, top left is position 1,1
void Lcd_Set_Cursor(uchar row, uchar col)
{
	uchar temp;
  RS = lcd_cmd;
	if(row == 1)
	{
    temp = row_1_offset + col - 1;
    lcd_write_byte(temp);
	}
	else if(row == two)
	{
		temp = row_2_offset + col - 1;
    lcd_write_byte(temp);
	}
}

/****************************************************************************/
//                    Wiggle the E line
void strobe(uchar cmd)      // shadow --> cmd
{
  cmd |= EN_on;             // EN 0000_0001
  write_lcd(cmd);           // send modified shadow to LCD
  cmd &= EN_off;            // EN 0b11111110
  write_lcd(cmd);           // send restored shadow to LCD
  PORTD = cmd;              // show activity ( for glitz))
}

/****************************************************************************/
//                          ADC module
void test_adc(void)
{
  uint result = null;
  uint result_avg = null;
  uchar i, j, buf[max_buf];
    
  Lcd_Clear();
  Lcd_Write_String("Turn the dial...");
  Lcd_Set_Cursor(line_2, col_1);
  Lcd_Write_String("ADC value = ");
  while(1)
  {
    result_avg = null;
    for(i = 0; i < max_samples; i++)    // get 30 samples
    {
      __delay_ms(acq_del);              // Acquisition delay 5ms
      GO = on;                          // Start conversion
      while(GO);                        // wait for conversion to complete
      result = (ADRESH << 8) + ADRESL;
      result_avg += result;
    }
    itoa(buf, result_avg / max_samples, max_buf);  // convert int to ascii
    Lcd_Set_Cursor(line_2,col_13);      // position cursor to print value
    RS = lcd_data;
    for(j = 0; buf[j] != '\0'; j++)
      lcd_write_byte(buf[j]);
    Lcd_Write_String("    ");           // erase residual stuff
  }
}

/****************************************************************************/
//              Bring the LCD to life in 4-bit mode
void lcd_init(void)
{
  uchar shadow = set_8_bit;   // 0b0011_0000
    
  write_lcd(shadow);          // data = 3, EN & RS lo  to PCF8574 --> LCD
  __delay_ms(power_up_delay);

  strobe(shadow);             // toggle EN line   latch PCF8574 output into LCD
  __delay_ms(init_delay_5);

  strobe(shadow);             // toggle EN line   latch PCF8574 output into LCD
  __delay_ms(init_delay_2);

  strobe(shadow);             // toggle EN line   latch PCF8574 output into LCD
  __delay_ms(init_delay_2);
    
  shadow = four_bit;
  write_lcd(shadow);          // data = 2, EN & RS lo  to PCF8574 --> LCD 
  strobe(shadow);             // toggle EN line   latch PCF8574 output into LCD
    
  lcd_write_byte(four_more);  // set 4 bit IF, move cursor
  lcd_write_byte(cursor);     // display ON, Cursor ON, no blink
  lcd_write_byte(inc_cursor); // Cursor increments, no shift
  Lcd_Clear();
}

/****************************************************************************/

void i2c_init(void)
{
  // Make all unused ports output and set to 0. Makes things more stable.
  PORTB =   all_hi;
  TRISC3 = 1;                   //Setting as input as given in datasheet
  TRISC4 = 1;                   //Setting as input as given in datasheet
  SSPCON =  master;   // set I2C master mode
  SSPADD =  bus100;   // 100kHz i2c bus with 20MHz xtal
  CKE = on;           // use I2C levels
  SMP = on;           // disable slew rate control
  SSPIF = off;        // clear SSPIF interrupt flag
  BCLIF = off;        // clear bus collision flag
}

/****************************************************************************/
