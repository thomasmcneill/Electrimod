/*! \file i2c.c
    \brief I2C Functions

	I2C Functions
*/

#include "pic.h"
#include "i2c.h"


unsigned long I2C_TimeOut;

void I2C_Init(int Khz,unsigned long TimeOutCycles)
{
	unsigned long BSR;
	I2C_TimeOut = TimeOutCycles;
	BSR = CLOCK_HZ;
	BSR = BSR / 4;
	BSR = BSR / Khz;
	BSR = BSR / 1000;
	BSR--;

	TRISCbits.RC3 = 1;			// Configure pins for input
	TRISCbits.RC4 = 1;

	SSPCON1 = 0b00101000;
	SSPCON2 = 0;
	SSPSTAT = 0;
	if(Khz < 400)
		SSPSTATbits.SMP = 1;

	
	SSPADD = BSR;
	PIR1bits.SSPIF = 0;
	PIR2bits.BCLIF = 0;

}

char I2C_WaitInterrupt(void)
{
	unsigned long counter = 0;
	do {    
		counter++;
		if(counter > I2C_TimeOut) {
			PIR1bits.SSPIF = 0;
			//Log_Error((FARROM*)"I2C Error:  Interrupt Timeout\r\n");
			return 1;
		}
			
	}	while(PIR1bits.SSPIF==0);
    PIR1bits.SSPIF=0;

	counter = 0;
	do {
		counter++;
		if(counter > I2C_TimeOut) {
			Log_Error((FARROM*)"I2C Error:  Idle Timeout\r\n");
			return 1;
		}

	} while ( ( SSPCON2 & 0x1F ) || ( SSPSTATbits.R_W ) );
    PIR1bits.SSPIF=0;
	SSPCON1bits.WCOL = 0;
	PIR2bits.BCLIF = 0;

	return 0;
}
 
void I2C_Start(void)
{
    SSPCON2bits.SEN=1;
	I2C_WaitInterrupt();
}

void I2C_Stop(void)
{
    SSPCON2bits.PEN=1;
    I2C_WaitInterrupt();
}
 
//Send restart bit
void I2C_Restart(void)
{
    SSPCON2bits.RSEN=1;
    I2C_WaitInterrupt();
}
 
char I2C_TX(unsigned char Byte)
{
	char t;
	char ack;
	//while(SSPSTATbits.BF);		// Wait if the buffer is full
	PIR1bits.SSPIF = 0;			// Clear interrupt again just in case.

	// Loop until the byte has been sent without collision
	do {
		SSPCON1bits.WCOL = 0;
		PIR2bits.BCLIF = 0;
    	SSPBUF=Byte;
	} while(!SSPCON1bits.WCOL);
    t = I2C_WaitInterrupt();
	ack = SSPCON2bits.ACKSTAT;
	if(t == 0 && ack == 0) {
		return 0;
	}
	if(t == 1) {
		//	Log_Error((FARROM*)"I2C Error:  I2C_TX Timeout on interrupt\r\n");
		return 2;
	}
	if(ack == 1) {
		//	Log_Error((FARROM*)"I2C Error:  I2C_TX Received NACK\r\n");
		return 1;
	}
	return 3;
}
 
int I2C_RX(void)
{
	unsigned char t;
    SSPCON2bits.RCEN=1;
    t = I2C_WaitInterrupt();
	if(t != 0 ) {
		Log_Error((FARROM*)"I2C Error:  I2C_RX Timeout on interrupt\r\n");
		return -1;
	}
	while(SSPCON2bits.RSEN);
	t = SSPBUF;
	return t;
	
}
 
void I2C_NACK(void)
{
    SSPCON2bits.ACKDT=1;
    SSPCON2bits.ACKEN=1;
    I2C_WaitInterrupt();
}
 
void I2C_ACK(void)
{
    SSPCON2bits.ACKDT=0;
    SSPCON2bits.ACKEN=1;
    I2C_WaitInterrupt();
}
 
 

