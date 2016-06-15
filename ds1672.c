/*! \file ds1672.c
    \brief DS1672 RTC Functions

	DS1672 RTC Functions
*/

#include "pic.h"
#include "i2c.h"

#define DS1672_WRITE_ADDRESS 0xd0
#define DS1672_READ_ADDRESS 0xd1
#define DS1672_TIMEOUT 500000



char DS1672_Zero(void)
{
	char t;
	char s;
	I2C_Init(90,1000000);
	I2C_Start();
	t=I2C_TX(0xD0);
	if(t!=0) {
		if(t==1)
			Log_Error((FARROM*)"DS1672: Zero Address NACK\r\n");
		if(t==2)
			Log_Error((FARROM*)"DS1672: Zero Address Timeout\r\n");

		return t;
	}
	t=I2C_TX(1);
	if(t!=0) {
		if(t==1)
			Log_Error((FARROM*)"DS1672: Zero Postion NACK\r\n");
		if(t==2)
			Log_Error((FARROM*)"DS1672: Zero Position Position Timeout\r\n");

	}

	for(s=0;s<6;s++)
	{
		t=I2C_TX(0);
		if(t!=0) {
			if(t==1)
				Log_Error((FARROM*)"DS1672: TX NACK\r\n");
			if(t==2)
				Log_Error((FARROM*)"DS1672: TX Timeout\r\n");
	
		}
	}
	I2C_Stop();	

}
unsigned long DS1672_Value(void)
{
	unsigned long value;
	int t;
	unsigned long v;
	I2C_Init(100,1000000);
	I2C_Start();
	t=I2C_TX(0xD1);
	if(t!=0) return 0xffffffff;

	t=I2C_RX();
	if(t==-1) return 0xffffffff;
	value=t;
	I2C_ACK();

	t=I2C_RX();
	if(t==-1) return 0xffffffff;
	v = t;
	value+=v<<8;
	I2C_ACK();

	t=I2C_RX();
	if(t==-1) return 0xffffffff;
	v = t;
	value+=v<<16;
	I2C_ACK();
	
	t=I2C_RX();
	if(t==-1) return 0xffffffff;
	v = t;
	value+=v<<24;
	I2C_NACK();

	I2C_Stop();	
	return value;

}
int DS1672_GetNextByte(void)
{
	int t;
	I2C_Init(100,1000000);
	I2C_Start();
	t=I2C_TX(DS1672_READ_ADDRESS);
	if(t!=0) {
		Log_Error((FARROM*)"DS1672: GetNextByte Error sending Address\r\n");
		return -1; 
	}
	t=I2C_RX();
	if(t==-1) {
		Log_Error((FARROM*)"DS1672: GetNextByte Error receiving byte\r\n");
		return -1;
	}
	I2C_NACK();
	I2C_Stop();	
	return t;

}
