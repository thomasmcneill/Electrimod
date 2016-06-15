/*! \file serial.c
    \brief UART TX and RX Functions

*/

#include "pic.h"
#include "serial.h"
#include "interrupts.h"

SERIAL_ERROR Serial_Status;		/// This structure provides access to error information for the serial ports

#pragma udata SERIAL_TX			// Put the next variable in to its own section
unsigned char Serial_TX_Buffer[DEVICE_SERIAL_PORTS][Serial_TX_Buffer_Size];	// Allocate the transmit buffer
unsigned char Serial_TX_Buffer_Start[DEVICE_SERIAL_PORTS];					// Start and End of the round 
unsigned char Serial_TX_Buffer_End[DEVICE_SERIAL_PORTS];						// robin TX buffer
unsigned char Serial_TX_Interrupts[DEVICE_SERIAL_PORTS];

#pragma udata SERIAL_RX			// Put the next variable in to its own section
unsigned char Serial_RX_Buffer[DEVICE_SERIAL_PORTS][Serial_RX_Buffer_Size];	// Allocate the receive buffer
unsigned char Serial_RX_Buffer_Start[DEVICE_SERIAL_PORTS];					// Start and End of the round
unsigned char Serial_RX_Buffer_End[DEVICE_SERIAL_PORTS];						// robin RX buffer
unsigned char Serial_RX_Interrupts[DEVICE_SERIAL_PORTS];

#pragma udata					// restore section naming


	

/////////////////////////////////////////////////////////////////////////////////////////
/// check the rx buffer for data
/////////////////////////////////////////////////////////////////////////////////////////
unsigned char Serial_RX_Data_Waiting(char port)
{
	if(Serial_RX_Interrupts[port])
		Interrupts_RX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
										//while the serial uart routine is accessing it
	
	if(Serial_RX_Buffer_Start[port] != Serial_RX_Buffer_End[port])		// If the two are not equal then we have data
	{
		if(Serial_RX_Interrupts[port])
			Interrupts_RX_Enable(port);	//Interrupt enable
		return 1;			//Return true since we have data
	} else {
		if(Serial_RX_Interrupts[port])
			Interrupts_RX_Enable(port);	//Interrupt enable
		return 0;			//Return false since we have no data
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
/// check the tx buffer for data
/////////////////////////////////////////////////////////////////////////////////////////
unsigned char Serial_TX_Data_Waiting(char port)
{
	if(Serial_TX_Interrupts[port])
		Interrupts_TX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
											//while the serial uart routine is accessing it
	
	if(Serial_TX_Buffer_Start[port] != Serial_TX_Buffer_End[port])				// If the two are not equal then we have data
	{
		if(Serial_TX_Interrupts[port])
			Interrupts_TX_Enable(port);	//Interrupt enable
		return 1;			// return true
	} else {
		if(Serial_TX_Interrupts[port])
			Interrupts_TX_Enable(port);	//Interrupt enable
		return 0;			//return false
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/// Put a character in the RX buffer
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_RX_PutChar(char port, unsigned char c)
{
	if(Serial_RX_Interrupts[port])
		Interrupts_RX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
										//while the serial uart routine is accessing it
		
	Serial_RX_Buffer[port][Serial_RX_Buffer_End[port]]=c;							// Set the last character
	Serial_RX_Buffer_End[port]++;												//Increment the end of the buffer position
	if(Serial_RX_Buffer_End[port] == Serial_RX_Buffer_Size) 					// See if we went past the end of the buffer
		Serial_RX_Buffer_End[port]=0;											// since it is we need to start it back at the beginning
	if(Serial_RX_Buffer_End[port] == Serial_RX_Buffer_Start[port])					// if the end and start are equal
	{																	// then we have to lose the oldest byte
		Serial_Status.RX_Overrun=1;										// The buffer was overrun
		Serial_RX_Buffer_Start[port]++;										// increment the start
		if(Serial_RX_Buffer_Start[port] == Serial_RX_Buffer_Size)				// if the start position is past the end of the buffer
			Serial_RX_Buffer_Start[port] = 0;									// set it back to the beginning
	} else {
		Serial_Status.RX_Overrun=0;
	}	
	if(Serial_RX_Interrupts[port])
		Interrupts_RX_Enable(port);	//Interrupt enable						
}
/////////////////////////////////////////////////////////////////////////////////////////
/// Put a CRLF
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_PutCRLF(char port)
{
	if(Serial_TX_Interrupts[port])
		Interrupts_TX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
											//while the serial uart routine is accessing it
		
	
	Serial_TX_Buffer[port][Serial_TX_Buffer_End[port]]=13;							// Set the last character value
	Serial_TX_Buffer_End[port]++;												// Increment the end of the buffer position
	if(Serial_TX_Buffer_End[port] == Serial_TX_Buffer_Size) 					// See if we went past the end of the buffer
		Serial_TX_Buffer_End[port]=0;											// since it is we need to set it back to the beginning
	if(Serial_TX_Buffer_End[port] == Serial_TX_Buffer_Start[port])					// if the end and start are equal
	{																	// then we have to lose the oldest byte
		Serial_Status.TX_Overrun=1;										// The buffer was overrun
		Serial_TX_Buffer_Start[port]++;										// increment the start
		if(Serial_TX_Buffer_Start[port] == Serial_TX_Buffer_Size)				// if the start position is past the end of the buffer
			Serial_TX_Buffer_Start[port] = 0;									// set it back to the beginning
	} else {
		Serial_Status.TX_Overrun=0;
	}	
	
	Serial_TX_Buffer[port][Serial_TX_Buffer_End[port]]=10;							// Set the last character value
	Serial_TX_Buffer_End[port]++;												// Increment the end of the buffer position
	if(Serial_TX_Buffer_End[port] == Serial_TX_Buffer_Size) 					// See if we went past the end of the buffer
		Serial_TX_Buffer_End[port]=0;											// since it is we need to set it back to the beginning
	if(Serial_TX_Buffer_End[port] == Serial_TX_Buffer_Start[port])					// if the end and start are equal
	{																	// then we have to lose the oldest byte
		Serial_Status.TX_Overrun=1;										// The buffer was overrun
		Serial_TX_Buffer_Start[port]++;										// increment the start
		if(Serial_TX_Buffer_Start[port] == Serial_TX_Buffer_Size)				// if the start position is past the end of the buffer
			Serial_TX_Buffer_Start[port] = 0;									// set it back to the beginning
	} else {
		Serial_Status.TX_Overrun=0;
	}	
	if(Serial_TX_Interrupts[port])
		Interrupts_TX_Enable(port);	//Interrupt enable
}
/////////////////////////////////////////////////////////////////////////////////////////
/// Put a character in the TX buffer
/////////////////////////////////////////////////////////////////////////////////////////
unsigned char Serial_PutChar(char port,unsigned  char c)
{
	if(Serial_RX_Interrupts[port])
			Interrupts_RX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
											//while the serial uart routine is accessing it

	

	Serial_TX_Buffer[port][Serial_TX_Buffer_End[port]]=c;							// Set the last character value
	Serial_TX_Buffer_End[port]++;												// Increment the end of the buffer position
	if(Serial_TX_Buffer_End[port] == Serial_TX_Buffer_Size) 					// See if we went past the end of the buffer
		Serial_TX_Buffer_End[port]=0;											// since it is we need to set it back to the beginning
	if(Serial_TX_Buffer_End[port] == Serial_TX_Buffer_Start[port])					// if the end and start are equal
	{																	// then we have to lose the oldest byte
		Serial_Status.TX_Overrun=1;										// The buffer was overrun
		Serial_TX_Buffer_Start[port]++;										// increment the start
		if(Serial_TX_Buffer_Start[port] == Serial_TX_Buffer_Size)				// if the start position is past the end of the buffer
			Serial_TX_Buffer_Start[port] = 0;									// set it back to the beginning
	} else {
		Serial_Status.TX_Overrun=0;
	}	
	if(Serial_RX_Interrupts[port])
		Interrupts_RX_Enable(port);	//Interrupt enable
}

/////////////////////////////////////////////////////////////////////////////////////////
/// Get next character from the RX buffer
/////////////////////////////////////////////////////////////////////////////////////////
unsigned char Serial_GetChar(char port)
{
	unsigned char c;
	if(Serial_RX_Interrupts[port])
		Interrupts_RX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
										//while the serial uart routine is accessing it
	if(Serial_RX_Buffer_Start[port] != Serial_RX_Buffer_End[port])			// First check for data waiting.  We could call the function but it is faster to duplicate the code here.
	{
		c=Serial_RX_Buffer[port][Serial_RX_Buffer_Start[port]];				// Get the character using the buffer start position
		Serial_RX_Buffer[port][Serial_RX_Buffer_Start[port]]=0;				// set the value to null
		Serial_RX_Buffer_Start[port]++;								// Increment the start of the buffer
		if(Serial_RX_Buffer_Start[port] == Serial_RX_Buffer_Size)		// If the start is at the end of the buffer
			Serial_RX_Buffer_Start[port] = 0;							// set the start to the beginning
		if(Serial_RX_Interrupts[port])
			Interrupts_RX_Enable(port);	//Interrupt enable					// enable the interrrupt
		return c;												// return the character
	} else {	
		if(Serial_RX_Interrupts[port])
			Interrupts_RX_Enable(port);	//Interrupt enable					// enable the interrupt
		return 0;												// return null even though there was no data
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
/// Get next character from the TX buffer
/////////////////////////////////////////////////////////////////////////////////////////
unsigned char Serial_TX_GetChar(char port)
{
	unsigned char c;
	if(Serial_TX_Interrupts[port])
		Interrupts_TX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
									//while the serial uart routine is accessing it
	if(Serial_TX_Buffer_Start[port] != Serial_TX_Buffer_End[port])
	{
		c=Serial_TX_Buffer[port][Serial_TX_Buffer_Start[port]];
		Serial_TX_Buffer[port][Serial_TX_Buffer_Start[port]]=0;
		Serial_TX_Buffer_Start[port]++;
		if(Serial_TX_Buffer_Start[port] == Serial_TX_Buffer_Size)
			Serial_TX_Buffer_Start[port] = 0;
		if(Serial_TX_Interrupts[port])
			Interrupts_TX_Enable(port);	//Interrupt enable
		return c;
	} else {
		if(Serial_TX_Interrupts[port])
			Interrupts_TX_Enable(port);	//Interrupt enable
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/// Read a character from the UART and put it in the RX buffer
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_Read_UART()
{
	char port;
	for(port=0;port<DEVICE_SERIAL_PORTS;port++)
	{
			if(port == 0)
			{
				Serial_Status.UART_FrameError = RCSTAbits.FERR;	// Check for framing errors
				Serial_Status.UART_Overrun = RCSTAbits.OERR;	// Check for uart overruns
				Serial_RX_Buffer[port][Serial_RX_Buffer_End[port]]=RCREG;				// Get the character from the UART
			}	
			Serial_RX_Buffer_End[port]++;							// Increment the end of the buffer
			if(Serial_RX_Buffer_End[port] == Serial_RX_Buffer_Size) 		// if the end pointer has passed the buffer end
				Serial_RX_Buffer_End[port]=0;						// send the end to the beginning of the buffer
			if(Serial_RX_Buffer_End[port] == Serial_RX_Buffer_Start[port])		// if the end and start are the same
			{
				Serial_RX_Buffer_Start[port]++;						// increment the start
				if(Serial_RX_Buffer_Start[port] == Serial_RX_Buffer_Size)	// if the start is past the end of the buffer
					Serial_RX_Buffer_Start[port] = 0;				// set it to the beginning
			}
			if(port == 0) {
				RCSTAbits.CREN=1;			// Enable receiver
				PIR1bits.RCIF=0;			// Clear the received character flag
			}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
/// Insert characters in to the RX buffer.  Used to simulate incoming data
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_Inject_RX_Buffer(char port,unsigned  char c)
{
	if(Serial_RX_Interrupts[port])
		Interrupts_RX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
										//while the serial uart routine is accessing it

	
	Serial_RX_Buffer[port][Serial_RX_Buffer_End[port]]=c;				// Get the character from the UART
	Serial_RX_Buffer_End[port]++;							// Increment the end of the buffer
	if(Serial_RX_Buffer_End[port] == Serial_RX_Buffer_Size) 		// if the end pointer has passed the buffer end
		Serial_RX_Buffer_End[port]=0;						// send the end to the beginning of the buffer
	if(Serial_RX_Buffer_End[port] == Serial_RX_Buffer_Start[port])		// if the end and start are the same
	{
		Serial_RX_Buffer_Start[port]++;						// increment the start
		if(Serial_RX_Buffer_Start[port] == Serial_RX_Buffer_Size)	// if the start is past the end of the buffer
			Serial_RX_Buffer_Start[port] = 0;				// set it to the beginning
	}
	if(Serial_RX_Interrupts[port])
		Interrupts_RX_Enable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
									//while the serial uart routine is accessing it
	

}
/////////////////////////////////////////////////////////////////////////////////////////
// Put a string in the transmit buffer
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_Inject_String_RX_Buffer(char port,unsigned  char *string)
{
	// Put data in outgoing buffer
	char t = 0;
	unsigned char c;

	do {
		c=string[t];
		if(c==0)		// if the character is null then return
			break;
		Serial_Inject_RX_Buffer(port,c);
		t++;
	} while (c != 0);
}
/////////////////////////////////////////////////////////////////////////////////////////
// Send all data from the TX buffer and put it in the UART
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_Send_Full_TX_Buffer(char port)
{
   
	if(port == 0)
	{
		if(Serial_TX_Interrupts[port]==0)
		{
			while (Serial_TX_Data_Waiting(port)) {		// Loop until all data is gone
				while (PIR1bits.TXIF==0) {}				// wait until the uart is done sending 
					TXREG=Serial_TX_GetChar(port);		
			}
		}	
	}
		
}

/////////////////////////////////////////////////////////////////////////////////////////
// Send one byte from the TX buffer and put it in the UART
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_Send_Byte_TX_Buffer(char port)
{
	if(port == 0)
	{   
		if(Serial_TX_Interrupts[port]==0)
		{
			if(Serial_TX_Data_Waiting(port) ) {		// Loop until all data is gone
				while (PIR1bits.TXIF==0) {}			// wait until the uart is done sending 
					TXREG=Serial_TX_GetChar(port);		
			}
		}
	}
		
}
/////////////////////////////////////////////////////////////////////////////////////////
// Send one byte from the TX buffer and put it in the UART
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_Send_UART()
{
	char port;
	for(port=0;port<DEVICE_SERIAL_PORTS;port++)
	{
		if(port == 0)
		{   
	
			if(Serial_TX_Data_Waiting(port) ) {		// Loop until all data is gone
				while (PIR1bits.TXIF==0) {}			// wait until the uart is done sending 
					TXREG=Serial_TX_GetChar(port);		
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// Put a string in the transmit buffer
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_Send_String(char port,unsigned  char *string,char sendnow)
{
	// Put data in outgoing buffer
	char t = 0;
	unsigned char c;

	do {
		c=string[t];
		if(c==0)		// if the character is null then return
			break;
		Serial_PutChar(port,c);
		t++;
	} while (c != 0);
	if(sendnow) 
		Serial_Send_Full_TX_Buffer(port);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Put a block of data in the transmit buffer
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_Send_Data(char port, unsigned char *Data, char length)
{
	// Put data in outgoing buffer
	unsigned char t;
	for(t=0;t<length;t++)
	{		
		Serial_PutChar(port, Data[t]);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// Take a snapshot of the RX buffer
/////////////////////////////////////////////////////////////////////////////////////////
void Serial_Input_Snapshot(char port,unsigned  char *buffer)
{
	char t;
	unsigned char Start=Serial_RX_Buffer_Start[port];			// Set the start to the beginning of the buffer
	unsigned char c;

	if(Serial_RX_Interrupts[port])
		Interrupts_RX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
	

		
	for(t=0;t<Serial_RX_Buffer_Size;t++)		// Loop for the size of the RX buffer
	{
		buffer[t]=Serial_RX_Buffer[port][Start];		
		Start++;						// Increment the pointer
		if(Start==Serial_RX_Buffer_Size)		// if the start has reached the end
			Start=0;					// go to the beginning
	}

	if(Serial_RX_Interrupts[port])
		Interrupts_RX_Disable(port);	//Interrupt Disable, we do this so we are not accessing the buffer 
}


	


//----------------------------------------------------------------------------
void Serial_SetupUART(char port, unsigned long baud, char RXInterrupt,char TXInterrupt)
{
	unsigned long b;
	unsigned char baudl;
	unsigned char baudh;

	
		TXSTAbits.TXEN = 0;
		RCSTAbits.SPEN = 0;			// Must be enabled to use the UART
	
		// Clear Buffer
		memset(Serial_RX_Buffer[port],0,Serial_RX_Buffer_Size);
		memset(Serial_TX_Buffer[port],0,Serial_TX_Buffer_Size);
	
		// Set the end and begginning pointers
		Serial_TX_Buffer_Start[port] = 0;
		Serial_TX_Buffer_End[port] = 0;
		Serial_RX_Buffer_Start[port] = 0;
		Serial_RX_Buffer_End[port] = 0;
	
	
	
	
		TRISCbits.TRISC7=1;			// Set direction for the TX and RX Pins
		TRISCbits.TRISC6=0;			// Set direction for the TX and RX Pins
	
		// Calculate the baudcon
		// ((FOSC/Baud)/64)-1
		b = CLOCK_HZ;
		b = b / baud;
		b = b / 4;
		b = b - 1;  
		baudh=b >> 8;
		baudl=b & 0xff;
		SPBRGH=baudh;
		SPBRG=baudl;
		Serial_Status.Baud_Actual = CLOCK_HZ / (4 * (b + 1)) ;
		Serial_Status.Baud = baud;
	
	
		TXSTAbits.TX9=0;			// Disable 9 bit serial 
		TXSTAbits.SYNC=0;			// 0 for Asynch and 1 for Synch
		TXSTAbits.SENDB=0;			// Disable sending one break character bit
		TXSTAbits.BRGH=1;			// 1 for high speed and 0 for low speed
	
		RCSTAbits.SREN=1;
		RCSTAbits.RX9=0;			// Set to 1 for 9 bit receiving
		RCSTAbits.ADDEN=0;			// Disable address detection
	
	
		BAUDCONbits.BRG16=1;  		// 16 Bit baud rate enabled
		BAUDCONbits.ABDEN=0;		// Disable autobaud

		TXSTAbits.TXEN=1;			// Enable Transmit
		RCSTAbits.SPEN = 1;			// Must be enabled to use the UART
		RCSTAbits.CREN=1;   		// Continuous receiving

	
		TXREG=0;					// Make sure there is nothing in the TXREG
	
		PIR1bits.RCIF=0;	// Clear RX data Flag
		if(RXInterrupt) {
			Interrupts_RX_Function[port]=Serial_Read_UART;
			Interrupts_RX_Enable(port);
		} else {
			Interrupts_RX_Function[port]=0;
			Interrupts_RX_Disable(port);
		}
		if(TXInterrupt) {
			Interrupts_TX_Function[port]=Serial_Send_UART;
			Interrupts_TX_Enable(port);
		}else {
			Interrupts_TX_Function[port]=0;
			Interrupts_TX_Disable(port);
		}
}



