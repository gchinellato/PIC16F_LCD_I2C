/*
 * File:   main.c
 * Author: ???
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "config.h"
#include "i2c.h"
#include "display.h"
#include "display-i2c.h"

//Loop period
#define LP 100

void main(void) 
{   
    //Init GPIO
    TRISDbits.TRISD6 = 0x00; // LED0 output
    TRISDbits.TRISD5 = 0x01; // BTN0 input
    PORTDbits.RD6 = 0x0; 
    
    //Init I2C
    I2C_Master_Init(100000);      //Initialize I2C Master with 100KHz clock
    __delay_ms(100);
    
    //Init display
    Display_Begin(DEVICE_ID, LCD_MAX_COLS, LCD_MAX_ROWS);
    backlight();
    noCursor();
    clear();
    setCursor(0,0);
    writeStr("  Projeto 2017");
    setCursor(0,1);
    writeStr("Henrique & Caio");
    __delay_ms(1000);
    
    unsigned int hits = 0;
    unsigned int timer0 = 0;
    unsigned int scaled = 0;
    
    clear();
    
    while(1)
    { 
        //clear();
        
        //toggle led
        PORTDbits.RD6 ^= (1 << 0);
        
        //100ms*15 = 1.5s
        if(timer0 >= (LP*15))
        {
            clear();  
            //random number position
            int max = LCD_MAX_COLS-1;
            int min = 0;
            scaled = (unsigned int)(rand() % (max-min)) + min;
            setCursor(scaled,0);
            writeChar('x');
            timer0 = 0;  
            hits=0;            
        }
  
        //button hits counter
        if(!PORTDbits.RD5)
        {            
            setCursor(hits,1);
            writeChar('o');
            hits++;
        }     

        if((scaled == hits) && ((scaled != 0) && (hits != 0)))
        {
           clear(); 
           hits=0;
           timer0=0;
           setCursor(0,0);
           writeStr("Ganhou o/o/");
           __delay_ms(2000);
           clear();
        }

        if(hits >= LCD_MAX_COLS)
        {
            hits=0;
        }
        
        __delay_ms(LP);
        timer0 += LP;   
    }
    return;
}