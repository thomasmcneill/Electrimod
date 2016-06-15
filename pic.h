/*! \file pic.h
    \brief PIC includes and configuration
*/

// Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// Hardware Includes
#include <pwm.h>
#include <timers.h>
#include <adc.h>
#include <delays.h>
#include <reset.h>


// Include for the PIC
#include <p18F2525.h>

#ifndef __pic_H
#define __pic_H 


#define CLOCK_HZ 			32000000  	/// This is here for proper timing calculation.
#define TIMER0_PRESCALER    256		  	/// Timer 0 prescaler
#define EEPROM_SIZE			1024		/// Size of EEPROM in bytes
#define FARROM const far rom char		/// This is used often to store text in ROM (FLASH)


////////////////////////////////////////////////
// PIN Definitions
////////////////////////////////////////////////
#define PIN0				PORTBbits.RB0
#define PIN1				PORTBbits.RB1
#define PIN2				PORTBbits.RB2
#define PIN3				PORTBbits.RB3
#define PIN4				PORTBbits.RB4
#define PIN5				PORTBbits.RB5
#define PIN6				PORTBbits.RB6
#define Status_LED			PORTCbits.RC5

void PIN0_ON(void);
void PIN1_ON(void);
void PIN2_ON(void);
void PIN3_ON(void);
void PIN4_ON(void);
void PIN5_ON(void);
void PIN6_ON(void);
void PIN0_OFF(void);
void PIN1_OFF(void);
void PIN2_OFF(void);
void PIN3_OFF(void);
void PIN4_OFF(void);
void PIN5_OFF(void);
void PIN6_OFF(void);

////////////////////////////////////////////////////
// Command Serial Definitions
////////////////////////////////////////////////////
#define ttyS0				0
#define ttyS1				1
#define ttyS2				2
#define ttyS3				3

#define COM0				0
#define COM1				1
#define COM2				2
#define COM3				3
#define NO_TX_INTERRUPTS    0
#define NO_RX_INTERRUPTS    0
#define TX_INTERRUPTS    	1
#define RX_INTERRUPTS    	1
#define SEND_NOW			1
#define SEND_LATER			0
#define I2C_MASTER			0
#define I2C_SLAVE			1
#define I2C_INTERRUPTS_ENABLED 1
#define I2C_INTERRUPTS_DISABLED 0
#define DEVICE_SERIAL_PORTS 1



///////////////////////////////////////////////////
/// Structure for holding status information
////////////////////////////////////////////////
typedef struct  {
	unsigned long	Loop_Cycles;			/// Configured when calling Configure_Loop.  Ticks per loop 
	unsigned char	TimeOut;				/// if the timer elapses we have a problem and this is set. 
	unsigned int  	Sequence;				/// Block number.  Number of iterations 
	unsigned int 	Loop_Latency;			/// This tracks how far off we are on the timing 
	int				Loop_Frequency;			/// 
} _STATUS_;
extern _STATUS_ Status;						/// Global status variable

extern char szText[128];						/// Variable used for creating strings for display.  Placed here due to memory limitations

//////////////////////////////////////////
/// Structure for abort codes
//////////////////////////////////////////
typedef struct  {
	unsigned	  User:1;
	unsigned      Example:1;
} _ABORT;
extern _ABORT Abort;


extern char STDERR;



void Initialize_PIC(void);
char BIT_GET(unsigned char *data, char i);
char BIT_SET(unsigned char *data, char i);
char BIT_CLEAR(unsigned char *data,char i);
void Log_Error(FARROM *string);
void CheckReset(char port);
#endif
