/*! \file ds1307.h
    \brief ds1307 includes
*/

// Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


// Include for the PIC
#include <p18F2525.h>

#ifndef __ds1307_H
#define __ds1307_H 

extern unsigned char DS1307_BUFFER[0x3f];


unsigned char DS1307_ToBCD(char in);
char DS1307_Disable(void);
char DS1307_Enable(void);
char DS1307_SetHour(char hour);
char DS1307_SetSeconds(char seconds);
char DS1307_SetMinutes(char minutes);
char DS1307_SetDay(char day);
char DS1307_SetDate(char date);
char DS1307_SetMonth(char month);
char DS1307_SetYear(char year);
char DS1307_WriteRAM(char addr,unsigned char c);
char DS1307_Read(void);


#endif
