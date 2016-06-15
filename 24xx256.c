/*! \file ds1672.c
    \brief EE24XX256  Functions

	EE24XX256  Functions
*/

#include "24xx256.h"
#include "pic.h"
#include "i2c.h"

#define EE24XX256_WRITE_ADDRESS 0b10100000
#define EE24XX256_READ_ADDRESS  0b10100001



char EE24XX256_WriteByte(char DeviceID, unsigned int Address, unsigned char B)
{
	char t;
	I2C_Init(100,100000);
	I2C_Start();
	t=I2C_TX(EE24XX256_WRITE_ADDRESS | (DeviceID << 1));
	if(t!=0) {
		if(t==1)
			Log_Error((FARROM*)"EE24XX256: Write DeviceID NACK\r\n");
		if(t==2)
			Log_Error((FARROM*)"EE24XX256: Write DeviceID Timeout\r\n");

		return t;
	}
	t=I2C_TX(Address >> 8);
	if(t!=0) {
		if(t==1)
			Log_Error((FARROM*)"EE24XX256: Write Address Byte MSB NACK\r\n");
		if(t==2)
			Log_Error((FARROM*)"EE24XX256: Write Address Byte MSB Timeout\r\n");
		return t;

	}
	t=I2C_TX(Address & 0xff);
	if(t!=0) {
		if(t==1)
			Log_Error((FARROM*)"EE24XX256: Write Address Byte LSB NACK\r\n");
		if(t==2)
			Log_Error((FARROM*)"EE24XX256: Write Address Byte LSB Timeout\r\n");
		return t;
	}

	t=I2C_TX(B);
	if(t!=0) {
		if(t==1)
			Log_Error((FARROM*)"EE24XX256: Write Address Byte LSB NACK\r\n");
	}
	I2C_Stop();	

	I2C_Init(100,1000000);
	I2C_Start();
	t=I2C_TX(EE24XX256_WRITE_ADDRESS | (DeviceID << 1));
	if(t!=0) {
		if(t==1)
			Log_Error((FARROM*)"EE24XX256: Write DeviceID (Wait for Write to complete) NACK\r\n");
		if(t==2)
			Log_Error((FARROM*)"EE24XX256: Write DeviceID (Wait for Write to complete) Timeout\r\n");

		return t;
	}
	return 0;
}
int EE24XX256_ReadByte(char DeviceID, unsigned int Address)
{
	int t;
	I2C_Init(100,1000000);
	I2C_Start();
	t=I2C_TX(EE24XX256_WRITE_ADDRESS | (DeviceID << 1));
	if(t!=0) {
		//if(t==1)
		//	Log_Error((FARROM*)"EE24XX256: ReadByte() DeviceID (Step 1) NACK\r\n");
		//if(t==2)
			//Log_Error((FARROM*)"EE24XX256: ReadByte() DeviceID (Step 1) Timeout\r\n");

		//return t;
	}
	t=I2C_TX(Address >> 8);
	if(t!=0) {
		//if(t==1)
			//Log_Error((FARROM*)"EE24XX256: ReadByte() Address Byte (Step 1) MSB NACK\r\n");
		//if(t==2)
			//Log_Error((FARROM*)"EE24XX256: ReadByte() Address Byte (Step 1) MSB Timeout\r\n");
		//return t;

	}

	t=I2C_TX(Address & 0xff);
	if(t!=0) {
		//if(t==1)
			//Log_Error((FARROM*)"EE24XX256: ReadByte() Address Byte (Step 1) LSB NACK\r\n");
		//if(t==2)
			//Log_Error((FARROM*)"EE24XX256: ReadByte() Address Byte (Step 1) LSB Timeout\r\n");
		//return t;
	}
	I2C_Restart();
	t=I2C_TX(EE24XX256_READ_ADDRESS | (DeviceID << 1));
	if(t!=0) {
		//if(t==1)
			//Log_Error((FARROM*)"EE24XX256: ReadByte() DeviceID (Step 2) NACK\r\n");
		//if(t==2)
			//Log_Error((FARROM*)"EE24XX256: ReadByte() DeviceID (Step 2) Timeout\r\n");

		//return t;
	}
	
	t=I2C_RX();
	if(t==-1) {
		//Log_Error((FARROM*)"EE24XX256: Read Timeout\r\n");
		 return t;
	}
	I2C_NACK();

	I2C_Stop();	
	return t;

}
int EE24XX256_ReadNextByte(char DeviceID)
{
	int t;
	I2C_Init(100,100000);

	I2C_Start();
	t=I2C_TX(EE24XX256_READ_ADDRESS | (DeviceID << 1));
	if(t!=0) {
		//if(t==1)
		//	Log_Error((FARROM*)"EE24XX256: ReadNextByte() DeviceID (Step 2) NACK\r\n");
		//if(t==2)
		//	Log_Error((FARROM*)"EE24XX256: ReadNextByte() DeviceID (Step 2) Timeout\r\n");

		return t;
	}
	
	t=I2C_RX();
	if(t==-1) {
		//Log_Error((FARROM*)"EE24XX256: ReadNextByte Timeout\r\n");
		 return t;
	}
	I2C_NACK();

	I2C_Stop();	
	return t;

}
