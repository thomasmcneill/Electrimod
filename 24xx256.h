/*! \file ee24xx256.h
    \brief ee24xx256 includes
*/

// Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


// Include for the PIC
#include <p18F2525.h>

#ifndef __ee24xx256_H
#define __ee24xx256_H 

char EE24XX256_WriteByte(char DeviceID, unsigned int Address, unsigned char B);
int EE24XX256_ReadByte(char DeviceID, unsigned int Address);
int EE24XX256_ReadNextByte(char DeviceID);




#endif
