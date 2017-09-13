#ifndef DISPLAY_H
#define	DISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "config.h"
    
// Pin out for LCD display (16 pins)
// ---------------------------------
// 1  - Gnd
// 2  - Vcc
// 3  - VContrast
// 4  - RS - P0 - Pin 4 PCF8574
// 5  - RW - P1 - Pin 5 PCF8574
// 6  - En - P2 - Pin 6 PCF8574
// 7  - D0 - Don't Care
// 8  - D1 - Don't Care
// 9  - D2 - Don't Care
// 10 - D3 - Don't Care
// 11 - D4 - P4 - Pin 9  PCF8574
// 12 - D6 - P5 - Pin 10 PCF8574
// 13 - D6 - P6 - Pin 11 PCF8574
// 14 - D7 - P7 - Pin 12 PCF8574
// 15 - Anode   LED
// 16 - Cathode LED
//
// PCF8574 register and pin mapping
// Bit 0 - RS  - P0 - Pin 4  PCF8574
// Bit 1 - RW  - P1 - Pin 5  PCF8574
// Bit 2 - En  - P2 - Pin 6  PCF8574
// Bit 3 - Led - P3 - Pin 7  PCF8574 (Active High, Led turned on)
// Bit 4 - D4  - P4 - Pin 9  PCF8574
// Bit 5 - D5  - P5 - Pin 10 PCF8574
// Bit 6 - D6  - P6 - Pin 11 PCF8574
// Bit 7 - D7  - P7 - Pin 12 PCF8574
//

#define DEVICE_ID  0x40
#define WRITE_FLAG   0b11111110
#define READ_FLAG    0b00000001
    
// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define Bl 0b00001000  // Backlight enable bit (On = 1, Off =0)
#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit
    
#define LCD_INIT      ((0b00000000 | En) & ~Rs) & (~Rw) // Used to set all the O/Ps on the PCF8574 to initialise the LCD
#define LCD_8BIT_INIT 0b00110000 // Used to initialise the interface at the LCD
#define LCD_4BIT_INIT 0b00100000 // Used to initialise the interface at the LCD

#define LCD_PCF8574_ADDR         (0x20<<1)  // Modify this if the default address is altered 
#define LCD_PCF8574_WEAK_PU      0b11110000 // Used to turn on PCF8574 Bits 7-4 on. To allow for read of LCD.

#define LCD_BUSY_FLAG_MASK       0b10000000 // Used to mask off the status of the busy flag
#define LCD_ADDRESS_COUNTER_MASK 0b01111111 // Used to mask off the value of the Address Counter
#define LCD_MAX_COLS             16
#define LCD_MAX_ROWS             2
    
#define LCD_LINE1	0x00		// Constant used to point to start of LCD Line 1
#define LCD_LINE2	0x40		// Constant used to point to start of LCD Line 2
#define LCD_LINE3	0x14		// Constant used to point to start of LCD Line 3
#define LCD_LINE4	0x54		// Constant used to point to start of LCD Line 4

    
void Display_Begin(int addr, int cols, int rows);

/************ low level data pushing commands **********/
void send(unsigned char value, unsigned char mode);
void write4bits(unsigned char value);
void expanderWrite(unsigned char data);
void pulseEnable(unsigned char data);
void setBacklight(unsigned char value);

/*********** mid level commands, for sending data/cmds */
void command(unsigned char value);
void write(unsigned char value);

/********** high level commands, for the user! */
void clear();
void home();
void setCursor(int col, int row);
void display();
void noDisplay();
void cursor();
void noCursor();
void blink();
void noBlink();
void scrollDisplayLeft();
void scrollDisplayRight();
void leftToRight();
void rightToLeft();
void autoscroll();
void noAutoscroll();
void backlight();
void noBacklight();
void writeChar(char value);
void writeStr(const char *message);

#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_H */

