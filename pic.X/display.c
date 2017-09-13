#include "display.h"
#include "i2c.h"
#include "config.h"

int	_addr = DEVICE_ID;
int	_cols = LCD_MAX_COLS;
int	_rows = LCD_MAX_ROWS;

int _displayfunction = 0;
int _displaycontrol = 0;
int _displaymode = 0;
int _backlightval = LCD_BACKLIGHT;

/* Set the LCD display in the correct begin state, must be called before anything else is done. */
void Display_Begin(int addr, int cols, int rows)
{
    _addr = addr;
    _cols = cols;
    _rows = rows;
    
    // Ensure supply rails are up before config sequence
    __delay_us(50000);
    
    //expanderWrite(_backlightval);
    expanderWrite(LCD_INIT);
    __delay_ms(1000);
    
    // Sequence to put the LCD into 4 bit mode this is according to the hitachi HD44780 datasheet page 109
    
    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03 << 4);
    __delay_us(4500);
   
    // second try
    write4bits(0x03 << 4);
    __delay_us(150);
    
    // third go!
    write4bits(0x03 << 4);
    __delay_us(150);
    
    // finally, set to 4-bit interface
     write4bits(0x02 << 4);
     __delay_us(150);
    
     //1 specify the number of display lines and character font
     //2 display off
     //3 display clear
     //4 entry mode set
     
     // set # lines, font size, etc.
     _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
     _displayfunction |= LCD_2LINE;
     command(LCD_FUNCTIONSET | _displayfunction);
     
     // turn the display on with no cursor or blinking default
     _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
     //display();   
     
     // clear it off
     //clear();
     
     // Initialize to default text direction (for roman languages)
     _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
     
     // set the entry mode
     command(LCD_ENTRYMODESET | _displaymode);
     
     //home();
}

/************ low level data pushing commands **********/
void send(unsigned char value, unsigned char mode)
{
    unsigned char highnib=value&0xf0;
    unsigned char lownib=(value<<4)&0xf0;
    write4bits((highnib)|En|mode);
    write4bits((lownib)|En|mode);
}

void write4bits(unsigned char value)
{
    expanderWrite(value & ~Rw);
    pulseEnable(value & ~Rw);
}

void expanderWrite(unsigned char data)
{
    I2C_Master_Start();
    I2C_Master_Write(DEVICE_ID);
    I2C_Master_Write(data | _backlightval);
    I2C_Master_Stop();    
}

void pulseEnable(unsigned char data)
{
    expanderWrite(data | En); // En high
    __delay_us(1);  // enable pulse must be >450ns
    
    expanderWrite(data & ~En); // En low
    __delay_us(50); // commands need > 37us to settle
}

void setBacklight(unsigned char value)
{
 	if (value) {
		backlight();		// turn backlight on
	} else {
		noBacklight();		// turn backlight off
	}   
}

/*********** mid level commands, for sending data/cmds */
void command(unsigned char value)
{
    send(value, 0);
}

void write(unsigned char value)
{
    send(value, Rs);
}

/********** high level commands, for the user! */
/* Remove all the characters currently shown. Next print/write operation will start 
 * from the first position on LCD display.
 */
void clear()
{
  	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	__delay_us(2000);  // this command takes a long time!  
}

/*
 * Next print/write operation will will start from the first position on the LCD display.
 */
void home()
{
    command(LCD_RETURNHOME);  // set cursor position to zero
	__delay_us(2000);  // this command takes a long time!
}

void setCursor(int col, int row)
{
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _rows) {
		row = _rows-1;    // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void display()
{
   _displaycontrol |= LCD_DISPLAYON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void noDisplay()
{
   _displaycontrol &= ~LCD_DISPLAYON;
   command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void cursor()
{
 	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);   
}
void noCursor()
{
 	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);   
}

// Turn on and off the blinking cursor
void blink()
{
    _displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void noBlink()
{
    _displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft()
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void scrollDisplayRight()
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight()
{
    _displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void rightToLeft()
{
    _displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void autoscroll()
{
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll()
{
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Turn the (optional) backlight off/on
void backlight()
{
    _backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}
void noBacklight()
{
    _backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

//print
void writeChar(char value)
{
    write((unsigned char)value);
}

void writeStr(const char *message)
{
    unsigned char *message_ptr = (unsigned char *) message;

    while (*message_ptr)
        write((unsigned char) (*message_ptr++));
}

