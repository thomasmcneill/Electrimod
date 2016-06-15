/*! \file i2c.h
    \brief i2c includes
*/

// Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// Include for the PIC
#include <p18F2525.h>

#ifndef __i2c_H
#define __i2c_H 


void I2C_SendStart(void);
void I2C_SendStop(void);
void I2C_ACK(void);
void I2C_NACK(void);
int I2C_RX(void);
char I2C_TX(unsigned char Byte);
void I2C_Restart(void);
void I2C_Stop(void);
void I2C_Start(void);
char I2C_WaitInterrupt(void);
void I2C_Init(int Khz,unsigned long TimeOutCycles);

extern unsigned long I2C_TimeOut;


#endif

