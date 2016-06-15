/*! \file ds1307.c
    \brief DS1307 RTC Functions

	DS1307 RTC Functions
*/

#include "pic.h"
#include "i2c.h"

#define DS1307_WRITE_ADDRESS 0b11010000
#define DS1307_READ_ADDRESS 0b11010001
#define DS1307_TIMEOUT 500000

#pragma udata DS1307	
unsigned char DS1307_BUFFER[0x3f];
#pragma udata

unsigned char DS1307_ToBCD(char in)
{
	unsigned char out_ones;
	unsigned char out_tens;
	unsigned char value;

	// First get rid of hundres place
	value = in / 100;
	in = in - (value * 100);

	out_tens = in / 10;
	
	in = in - (value * 10);

	out_ones = in;
	out_tens = out_tens << 4;
	return (out_ones + out_tens);
}
unsigned char DS1307_FromBCD(char in)
{
	unsigned char out;
	unsigned char ones;
	unsigned char tens;
	
	ones = in & 0x0f;
	tens = (in >> 4)*10;
	return ones + tens;
}

	

char DS1307_Disable(void)
{
	unsigned char s[2] = { 0,0};
	BIT_SET(&s[1],7);
	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&s,2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;
}
char DS1307_Enable(void)
{
	unsigned char s[2] = {0,0};
	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&s,2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;
}
char DS1307_SetHour(char hour)
{
	unsigned char b[2];
	b[0] = 0x02;
	b[1] = DS1307_ToBCD(hour);
	BIT_SET(&b[1],6);
	
	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&b[0],2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;

}

char DS1307_SetSeconds(char seconds)
{
	unsigned char b[2];
	b[0] = 0x00;
	b[1] = DS1307_ToBCD(seconds);
	
	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&b[0],2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;

}
char DS1307_SetMinutes(char minutes)
{
	unsigned char b[2];
	b[0] = 0x01;
	b[1] = DS1307_ToBCD(minutes);
	
	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&b[0],2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;

}
char DS1307_SetDay(char day)
{
	unsigned char b[2];
	b[0] = 0x03;
	b[1] = DS1307_ToBCD(day);
	
	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&b[0],2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;
}
char DS1307_SetDate(char date)
{
	unsigned char b[2];
	b[0] = 0x04;
	b[1] = DS1307_ToBCD(date);
	
	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&b[0],2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;
}
char DS1307_SetMonth(char month)
{
	unsigned char b[2];
	b[0] = 0x05;
	b[1] = DS1307_ToBCD(month);
	
	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&b[0],2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;
}
char DS1307_SetYear(char year)
{
	unsigned char b[2];
	b[0] = 0x06;
	b[1] = DS1307_ToBCD(year);
	
	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&b[0],2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;
}
char DS1307_WriteRAM(char addr,unsigned char c)
{
	unsigned char b[2];
	if(addr < 0x3f) {
		b[0] = 0x08 + addr;
		b[1] = c;
	
		if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&b[0],2, DS1307_TIMEOUT,100)==0)
			return 1; // Success
		else
			return 0; // Failed;
		}
	return 0;
}

char DS1307_Read(void)
{
	unsigned char b[2] = {0x7,0x0};
	// First we write to the control register to set our location
	// Writing anywhere else would adjust the time or change a value

	if(I2C_Master7_WriteBytes(DS1307_WRITE_ADDRESS,&b[0],2, DS1307_TIMEOUT,100)==0)
		return 1; // Success
	else
		return 0; // Failed;

	if(I2C_Master7_ReadBytes(DS1307_READ_ADDRESS,&DS1307_BUFFER[0],0x3f, DS1307_TIMEOUT,100)==0)
		return 1; // success
	else
		return 0; // failed
	
}
char DS1307_GetSeconds(void)
{
	return(DS1307_FromBCD(DS1307_BUFFER[0x3f - 7]));	

}
char DS1307_GetMinutes(void)
{
	return(DS1307_FromBCD(DS1307_BUFFER[0x3f - 6]));	

}
char DS1307_GetHours(void)
{
	return(DS1307_FromBCD(DS1307_BUFFER[0x3f - 5]));	
}
char DS1307_GetN(char c)
{
	c=c - 0x7;
	if(c < 0)	
		c = c + 0x3f;

	return(DS1307_BUFFER[c]);	
}
