/*! \file pic.c
    \brief Initialization functions
*/
#include "config.h"

#include "pic.h"
#include "serial.h"
#include "interrupts.h"
#include "timer.h"

_STATUS_ Status;
_ABORT Abort;
char STDERR;

void PIN0_ON(void) { PIN0 = 1; }
void PIN1_ON(void) { PIN1 = 1; }
void PIN2_ON(void) { PIN2 = 1; }
void PIN3_ON(void) { PIN3 = 1; }
void PIN4_ON(void) { PIN4 = 1; }
void PIN5_ON(void) { PIN5 = 1; }
void PIN6_ON(void) { PIN6 = 1; }
void PIN0_OFF(void) { PIN0 = 0; }
void PIN1_OFF(void) { PIN1 = 0; }
void PIN2_OFF(void) { PIN2 = 0; }
void PIN3_OFF(void) { PIN3 = 0; }
void PIN4_OFF(void) { PIN4 = 0; }
void PIN5_OFF(void) { PIN5 = 0; T1CONbits.TMR1ON = 0; }
void PIN6_OFF(void) { PIN6 = 0; }

void Initialize_PIC() 
{
	int t;

	Interrupts_Init();

	STDERR = -1;				/// Set Standard Error an undefined serial port;

	memset(&Status,0,sizeof(Status));		// Clear all the values
	memset(&Abort,0,sizeof(Abort));


	TRISB=0x00;	// Set Direction of B
	TRISC=0x00; // Set Direction of C
	PORTC=0x00;	// Set all port C to low
	PORTB=0x00;	// Setall port B to low
	
	


	Delay1KTCYx(10);

}
/////////////////////////////////////////////////
/// Get the value of a bit in a char
/////////////////////////////////////////////////
char BIT_GET(unsigned char *data, char i)
{
	return (*data >> i) & 0x01;

}
/////////////////////////////////////////////////
/// Set the value of a bit in a char
/////////////////////////////////////////////////
char BIT_SET(unsigned char *data, char i)
{
	*data |= 1 << i;
}
/////////////////////////////////////////////////
/// Clear a bit in a char
/////////////////////////////////////////////////
char BIT_CLEAR(unsigned char *data, char i)
{
	*data &= ~(1 << i);
}

void Log_Error(FARROM *string)
{
	char t=0;
	char c=0;	
	if(STDERR > -1) {
		do {
			c = string[t];
			Serial_PutChar(STDERR,c);
			t++;
		} while(c!=0);
		Serial_Send_Full_TX_Buffer(STDERR);
	}
}

void CheckReset(char port)
{
	
	////////////////////////////////////////////////////////////
	// If the PIC had a reset find out what caused it		
	////////////////////////////////////////////////////////////
	if(isBOR()) {
		
		sprintf(szText,(FARROM*)"Brown Out=Yes\r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	} else {
		sprintf(szText,(FARROM*)"Brown Out=No \r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	}
	if(isLVD()) {
		
		sprintf(szText,(FARROM*)"LVD=Yes\r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	} else {
		sprintf(szText,(FARROM*)"LVD=No \r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	}
	if(isMCLR()) {
		
		sprintf(szText,(FARROM*)"MCLR=Yes\r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	} else {
		sprintf(szText,(FARROM*)"MCLR=No \r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	}
	if(isPOR()) {
		
		sprintf(szText,(FARROM*)"POR=Yes\r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	} else {
		sprintf(szText,(FARROM*)"POR=No \r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	}

	if(isWDTTO()) {
		
		sprintf(szText,(FARROM*)"WDT=Yes\r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	} else {
		sprintf(szText,(FARROM*)"WDT=No \r\n");
		Serial_Send_String(port,szText,SEND_NOW);
	}
	Delay10KTCYx(255);

}

