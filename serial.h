#ifndef __SERIAL_H
#define __SERIAL_H 

#define Serial_RX_Buffer_Size 32				// Define the receive buffer size
#define Serial_TX_Buffer_Size 64				// Define the transmit buffer size


typedef struct  {
	unsigned	  RX_Overrun:1;
	unsigned	  TX_Overrun:1;
	unsigned      UART_FrameError:1;
	unsigned      UART_Overrun:1;
	unsigned      Baud;
	unsigned      Baud_Actual;
} SERIAL_ERROR;

extern SERIAL_ERROR Serial_Status;

extern unsigned char Serial_RX_Interrupts[DEVICE_SERIAL_PORTS];
extern unsigned char Serial_TX_Interrupts[DEVICE_SERIAL_PORTS];
extern unsigned char Serial_Ports_Active[DEVICE_SERIAL_PORTS];




extern unsigned char Serial_TX_Buffer[DEVICE_SERIAL_PORTS][Serial_TX_Buffer_Size];	// Allocate the transmit buffer
extern unsigned char Serial_TX_Buffer_Start[DEVICE_SERIAL_PORTS];					// Start and End of the round 
extern unsigned char Serial_TX_Buffer_End[DEVICE_SERIAL_PORTS];						// robin TX buffer

extern unsigned char Serial_RX_Buffer[DEVICE_SERIAL_PORTS][Serial_RX_Buffer_Size];	// Allocate the receive buffer
extern unsigned char Serial_RX_Buffer_Start[DEVICE_SERIAL_PORTS];					// Start and End of the round
extern unsigned char Serial_RX_Buffer_End[DEVICE_SERIAL_PORTS];						// robin RX buffer

/////////////////////////////////////////////////////////////////////////////////////////////
// User functions
/////////////////////////////////////////////////////////////////////////////////////////////
void Serial_SetupUART(char port,unsigned long baud, char RXInterrupt,char TXInterrupt);
unsigned char Serial_RX_Data_Waiting(char port);				// See if the receive buffer has any data in it
unsigned char Serial_TX_Data_Waiting(char port);				// See if the transmit buffer has any data in it
unsigned char Serial_PutChar(char port,unsigned  char c);					// Put a character in the transmit buffer
unsigned char Serial_GetChar(char port);							// Get a character from the receive buffer
void Serial_Send_String(char port,unsigned char *string, char sendnow);			// Send a string to the tx buffer
void Serial_Send_Data(char port, unsigned char *Data, char length);	// Send a block of data to the TX buffer
void Serial_Input_Snapshot(char port, unsigned char *buffer);
void Serial_Send_Byte_TX_Buffer(char port);				// Send one byte from the tx buffer to the tx uart
void Serial_Send_Full_TX_Buffer(char port);				// Send full tx buffer to the tx uart
void Serial_Inject_RX_Buffer(char port,unsigned  char c);
void Serial_Inject_String_RX_Buffer(char port, unsigned char *string);
void Serial_PutCRLF(char port);


/////////////////////////////////////////////////////////////////////////////////////////////
// Backend functions
/////////////////////////////////////////////////////////////////////////////////////////////
void Serial_RX_PutChar(char port,unsigned  char c);					// Put a character at the end of the rx buffer
unsigned char Serial_TX_GetChar(char port);						// Get a character from the beginning of the tx buffer
void Serial_Read_UART();					// Read a character from the UART and put it in the RX buffer
void Serial_Send_UART();					// Read a character from the UART and put it in the RX buffer
unsigned char Serial_CheckPort(char port);







#endif
