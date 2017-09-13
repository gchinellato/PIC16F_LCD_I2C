#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "config.h"

void I2C_Master_Init(const unsigned long c);
void I2C_Master_Wait();
void I2C_Master_Start();
void I2C_Master_RepeatedStart();
void I2C_Master_Stop();
void I2C_Master_Write(unsigned char d);
void I2C_Ack();
void I2C_Nak();
unsigned short I2C_Master_Read(unsigned short a);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

