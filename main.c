#include "pic.h"
#include "serial.h"
//#include "commandprompt.h"
//#include "bootloader.h"
#include "loop.h"
#include "timer.h"
#include "pwm.h"
#include "servo.h"
#include "eeprom.h"
#include "interrupts.h"
#include "ansi.h"
#include "a2d.h"
//#include "math.h"

void Process_Serial_Single_Commands(void);
void DoAbort(void);
void Compose_Output(void);
void test(void);

void Ignition(void);
void Ignition_SparkOn(void);
void Ignition_SparkOff(void);
void Ignition_InjectorOn(void);
void Ignition_InjectorOff(void);
void Read_Config(void);
void Draw_Screen(void);
void RPM_Overflow(void);


char    szText[128];			// Varaible used for creating strings.  Placed here due to memory limits on the chip

typedef struct {
	int Throttle_Position;				// Initial Angle
	int Throttle_Factor;				// Initial value for changing angle position
										//    this value may change if the system
										//    is too slow to compensate during transitions
										//    from light load to heavy load.


	int RPM_Current;					// Initial RPM
	int RPM_Target;						// Target RPM
	int RPM_Difference;					// Difference between the two
	long RPM_TMR3_Counter;				// Counter to calculate RPM.  Incremented every
										// 0.0001 seconds
	char RPM_TMR3_Counter_Over_Limit;		// If the Counter goes over RPM_OVERFLOW_LIMIT
										// then this is set to true
	
	unsigned int Spark_Duration;		// microseconds spark duration			
	unsigned int Spark_Frequency;		// Spark PWM frequency
	char         Spark_DutyCycle;		// Spark Duty Cycle (0-100%)
	unsigned int Spark_Delay;			// Spark Delay.  This is how long we wait after the timing
	int			 Spark_Correction; 		//    pulse before we start the pulse
	
	
	unsigned int Injector_MinDuration;	// Timing in uS for injector duration
	unsigned int Injector_Factor;		// This is a multiplier used to calculate the pulse
										//    length based on current Throttle Position
	unsigned int Injector_Duration;		// Current workig duration value

	int Injector_Correction;					// 10 bit value from the a2d.  0 - 1024
										// the value is split from -511 + 511
} _LAWN_MOWER;

_LAWN_MOWER Lawn_Mower;

#define SPARK_PIN 					PIN3		// RB3
#define SPARK_GAIN_PIN_NAME  		AN1			// AN1
#define SPARK_uS_INDEX_START		0
#define SPARK_uS_INDEX_STOP			1
#define SPARK_MINIMUM_DELAY			250			// Min spark delay
#define THROTTLE_PIN 				5			// RB5
#define THROTTLE_PIN_NAME			PIN5		// RB5
#define INJECTOR_PIN 				PIN4		// RB4
#define THROTTLE_FACTOR				25			// Multiplier for throttle position.  This value is divided by 10000, so a value of 100 is actually 0.01.  This avoids floating point.
#define INJECTOR_uS_INDEX_STOP		2
#define INJECTOR_GAIN_PIN_NAME		AN0			// AN0
#define INJECTOR_MIN_DURATION		1000	
#define INJECTOR_FACTOR				100			// Multiplier for injector open time.  This value is divided by 1000, so a value of 100 is actually 0.1.  This avoids floating point.
#define THROTTLE_SERVO 				0	// number in servo index.  Unique to each servo the system is controlling
#define SCREEN_UPDATE_FREQUENCY 	10	 
#define RPM_UPDATE_FREQUENCY 		1  
#define RPM_TARGET					3600
#define LOOP_FREQUENCY				100			

#define FREQ_GEN_PIN_NAME			PIN6

#define uS_TIMER_THROTTLE		0
#define uS_TIMER_SPARK_ON		1
#define uS_TIMER_SPARK_OFF		2
#define uS_TIMER_INJECTOR_OFF	3
#define uS_TIMER_FREQ_GEN		4
#define RPM_COUNTER_LIMIT  	10000			// RPM counter is updated every 0.0001 seconds
												// A count of 10000 is 1 seconds.  If no spark
												// has happened in 1 second then we need to
												// stop ignition, like spark and fuel

void main (void)
{
	char ScreenUpdateCounter = 0;		// I use this to change the frequency for the screen updates
	char IgnitionUpdateCounter = 0;		// Used to update the engine calculations but we don't need to do it at full loop frequency
	char tempChar;
	
	///------------------------------------------------
	// Set the initial parameters for functioning
	///------------------------------------------------
	Initialize_PIC();		


	///-------------------------------------------------------------------------------------------
	/// Set up communications
	///-------------------------------------------------------------------------------------------
	STDERR = COM0;			// Make all errors go out the first serial port
	Serial_SetupUART(COM0,38400,RX_INTERRUPTS,NO_TX_INTERRUPTS);	// Configure the first serial port
																	// Interrupts for RX data
																	// No interrupts for TX data.  The output
																	// is not really a priority.  Receiving commands
																	// is very important.  TX data can be forced to
																	// send immediately but it is a blocking call.

	Console_reset(COM0);											// Reset the VT100 Console
	Console_cls(COM0);												// Clear the VT100 console screen
	Console_Attribute(COM0,CONSOLE_ATTR_NORMAL);					// Make sure the text is normal


	Serial_Send_Full_TX_Buffer(COM0);								// Flush the TX buffer, blocking call
	///--------------------------------------------------------
	/// Read the configuration from the EEPROM
	///--------------------------------------------------------
	Read_Config();			

	///--------------------------------------------------------
	/// This functions displays the reason for the last reset.  Only good
	/// if the micontroller reset for some unknown reason
	///--------------------------------------------------------
	CheckReset(STDERR);		// Check to see if the system had been reset and display the reason

	///--------------------------------------------------------
	/// Draw the initial screen
	///--------------------------------------------------------
	Draw_Screen();

	///-------------------------------------------------------------------------------------------
	/// Configure initial parameters for the lawn mower structure
	///-------------------------------------------------------------------------------------------
	Lawn_Mower.Injector_Duration=Lawn_Mower.Injector_MinDuration;
	Lawn_Mower.RPM_TMR3_Counter = 0;
	Lawn_Mower.RPM_Current = 0;
	Lawn_Mower.RPM_TMR3_Counter_Over_Limit=1;			// We don't know if the engine is turning so start off th this flag
	Lawn_Mower.RPM_Difference = Lawn_Mower.RPM_Target - Lawn_Mower.RPM_Current;
	Lawn_Mower.Injector_Correction = 0;
	Lawn_Mower.Spark_Correction = 0;

	///-------------------------------------------------------------------------------------------
	/// Initialize A2D
	///-------------------------------------------------------------------------------------------
	A2D_Init();

	///-------------------------------------------------------------------------------------------
	/// Configure the PWM output for the spark plug
	///-------------------------------------------------------------------------------------------
	PWM_Configure(0, Lawn_Mower.Spark_Frequency, Lawn_Mower.Spark_DutyCycle);

	///-------------------------------------------------------------------------------------------
	/// Configure the servo to run but not in auto mode
	///-------------------------------------------------------------------------------------------
	Servo_Init(0 /* repeat interval (ms) */
			,-1 /* uS Timer Channel */ );		// use channel -1 to instruct it not to autorun

	Servo_Add(THROTTLE_SERVO /* index */
		,THROTTLE_PIN /* pin */	
		,45 /* start angle */
		,90 /* max angle */	
		,450 /* min timing pulse */
		,1850 /* max timing pulse */
		,uS_TIMER_THROTTLE /* us Channel */		); 
	
	///-------------------------------------------------------------------------------------------
	/// Configure the main program loop timer
	///-------------------------------------------------------------------------------------------
	Loop_Configure(LOOP_FREQUENCY);				
	
	///-------------------------------------------------------------------------------------------
	/// Configure the interrupt on port 0 to watch for the magnet
	/// the magnet sends a pulse as it passes
	/// this function starts the spark process and injector
	/// and it also calculates the RPM based on Timer3 values
	///-------------------------------------------------------------------------------------------
	TRISBbits.RB0 = 1;
	Interrupts_Pin0_Function = Ignition;
	Interrupts_Pin0_Enable();

	///-------------------------------------------------------------------------------------------
	/// Configure the microsecond timer
	///-------------------------------------------------------------------------------------------
	Timers_Timer1_uS_Queue_Init();
	Timers_Timer1_uS_Queue_Start();

	///-------------------------------------------------------------------------------------------
	/// Configure timer3 to calculate RPM.  If the timer overflows then we know 
	/// the engine isn't turning.
	///-------------------------------------------------------------------------------------------
	Timers_Timer3_Enable(1, 8);
	Interrupts_Timer3_Function=RPM_Overflow;
	Interrupts_Timer3_Enable();
	Timers_Timer3_SetValue(65435); 		// Increment it every 0.0001 seconds
	//PIE2bits.TMR3IE = 0;

	///-------------------------------------------------
	/// Start the infinite loop
	///-------------------------------------------------
  	while (1)
    {
		///-------------------------------------------------
		/// check to see if the loop timer has lapsed
		///-------------------------------------------------
		if(Loop_Next())		
		{						

			///-----------------------------------------------------------------------------
			/// Read the two knobs and get the correction factors for the fuel and spark
			///------------------------------------------------------------------------------
			Lawn_Mower.Injector_Correction = A2D_GetValue(INJECTOR_GAIN_PIN_NAME) - 512;	// The values can be from -511 to 512 based on a 10bit A2D (1024)			
			Lawn_Mower.Spark_Correction = A2D_GetValue(SPARK_GAIN_PIN_NAME) - 512;

			// Calculate the RPM Difference
			Lawn_Mower.RPM_Difference = Lawn_Mower.RPM_Target - Lawn_Mower.RPM_Current;

			// The RPM is too Low so we need to increase the throttle angle
			if(Lawn_Mower.RPM_Difference > 0) 	{
				// The value is increased based on the rpm difference multiplied by a scaling value
				Lawn_Mower.Throttle_Position+=(Lawn_Mower.Throttle_Factor * Lawn_Mower.RPM_Difference) / 10000;	

				// Dont let the angle go over 90
				if(Lawn_Mower.Throttle_Position > 90) {
					Lawn_Mower.Throttle_Position = 90;
				}
			} else {							
			// The RPM is too low
				Lawn_Mower.Throttle_Position-=(Lawn_Mower.Throttle_Factor * Lawn_Mower.RPM_Difference) / 10000;
				if(Lawn_Mower.Throttle_Position < 0) {
					Lawn_Mower.Throttle_Position = 0;
				}
			}

				// Calculate the length of the injector pulse which is based on the throttle position multipled by a factor value
				Lawn_Mower.Injector_Duration = 	Lawn_Mower.Injector_MinDuration + Lawn_Mower.Throttle_Position * Lawn_Mower.Injector_Factor;
				if(Lawn_Mower.Injector_Correction < 0 && (Lawn_Mower.Injector_Correction*-1) > Lawn_Mower.Injector_Duration) {
						Lawn_Mower.Injector_Duration = 0;
				} else {
					Lawn_Mower.Injector_Duration+= Lawn_Mower.Injector_Correction;
				}

				Servo_SetAngle(THROTTLE_SERVO,Lawn_Mower.Throttle_Position);
				Servo_SetAngle(THROTTLE_SERVO,tempChar);
				Servo_Update_Func();
					
				tempChar++;
				if(tempChar==90)
					tempChar=0;
				////////////////////////////////////////////////////
				//  Blink the status LED
				////////////////////////////////////////////////////
				if(Status_LED)
					Status_LED=0;
				else
					Status_LED=1;



			
			




				
			/////////////////////////////////////////////////////
			// Send output strings to terminal
			/////////////////////////////////////////////////////
			if(ScreenUpdateCounter==0)
				Compose_Output();
			ScreenUpdateCounter++;
			if(ScreenUpdateCounter == SCREEN_UPDATE_FREQUENCY)
				ScreenUpdateCounter = 0;

			
		}
		Serial_Send_Full_TX_Buffer(COM0);		// Send UART buffer
		Process_Serial_Single_Commands();	// Process Input from terminal

    } // While Loop for continuous running
	Serial_Send_Full_TX_Buffer(COM0);

}
void Read_Config(void)
{
	///-------------------------------------------------------------------------------------------
	/// Read configuration from EEPROM.  If CRC doesn't match set the following default params
	///-------------------------------------------------------------------------------------------
	if( !EEPROM_Read_CRC(0, &Lawn_Mower,sizeof(_LAWN_MOWER))) 
	{
		Lawn_Mower.Throttle_Position = 0;
		Lawn_Mower.Throttle_Factor = THROTTLE_FACTOR;		// Really 0.025.  The values are scaled by 1000 to avoid floating point

		Lawn_Mower.RPM_Target = RPM_TARGET;

		Lawn_Mower.Spark_Duration = 1000;
		Lawn_Mower.Spark_Frequency = 5000;
		Lawn_Mower.Spark_DutyCycle = 25;
		Lawn_Mower.Spark_Delay = SPARK_MINIMUM_DELAY;
	
		Lawn_Mower.Injector_MinDuration = INJECTOR_MIN_DURATION;
		Lawn_Mower.Injector_Factor = INJECTOR_FACTOR;

		sprintf(szText,(FARROM*)"Parameters could not be read from EEPROM\r\n");
		Serial_Send_String(COM0,szText,SEND_NOW);
		
	} else {	
		sprintf(szText,(FARROM*)"Read parameters from EEPROM\r\n");
		Serial_Send_String(COM0,szText,SEND_NOW);
	}

}



///////////////////////////////////////////////////////
// If timer3 overflows then the engine is not running
// so we need to set the value to -1 
// so the calculation knows it can't
// calculate until it gets a second pulse
//////////////////////////////////////////////
void RPM_Overflow(void)
{
	Lawn_Mower.RPM_TMR3_Counter++;							// Increment counter
	if(Lawn_Mower.RPM_TMR3_Counter > RPM_COUNTER_LIMIT)		// we hit the limit so set the flag
	{
		Lawn_Mower.RPM_Current = 0;							// we have no RPM or way too slow RPM
		Lawn_Mower.RPM_TMR3_Counter = 0;						// Reset Counter
		Lawn_Mower.RPM_TMR3_Counter_Over_Limit=1;			// set over limit flag

	}
	Timers_Timer3_SetValue(65435); 		// Increment it every 0.0001 seconds
}

//////////////////////////////////////////////////////
// This functions watches the magnet
// if the magnet passes it needs to fire the spark
// and pulse the injector.  It calculates
// the RPM based on time difference between pulses.
//////////////////////////////////////////////////
void Ignition(void)
{
	Lawn_Mower.RPM_Current = 0;
	if(	Lawn_Mower.RPM_TMR3_Counter_Over_Limit == 0) {	// if the over limit flag is not set calulate the RPM
		// 1 x RPM_TM3_Counter = 0.0001 seconds
		// RPM is 60 x rotations per second
		// We don't want to use float so we multiply both RPM_TMR_Counter and 60 by 10000
		// The RPM_TMR3_Counter is conviently incremented by 1 every 0.0001 so that multiply is
		// already done.  So we multiply 60 x 10000 and divide by the counter
		Lawn_Mower.RPM_Current = 600000 / Lawn_Mower.RPM_TMR3_Counter;
	}
	Lawn_Mower.RPM_TMR3_Counter_Over_Limit=0;
			
	// Start the timer over
	Lawn_Mower.RPM_TMR3_Counter = 0;
	Timers_Timer3_SetValue(65435);
	
	// Make sure the PWM pin for the spark plug is off
	SPARK_PIN=0;					
	
	// Open the injector
	INJECTOR_PIN = 1;;			
	
	// Set the injector to turn off after the appropriate time
	Timers_Timer1_uS_Queue_Add(uS_TIMER_INJECTOR_OFF,Ignition_InjectorOff,50000,0,1);	

	// Set the delay for the spark plug
	Timers_Timer1_uS_Queue_Add(uS_TIMER_SPARK_ON,Ignition_SparkOn,Lawn_Mower.Spark_Delay+Lawn_Mower.Spark_Correction,0,1);


}

/////////////////////////////////////////
// This function turns on the spark plug 
// by enableding the pwm
// and then schedules the pwm to turn off
////////////////////////////////////////
void Ignition_SparkOn(void)
{
	// Start the PWM
	PWM_Start();

	// Set the timer to disable the pwm after the required duration
	Timers_Timer1_uS_Queue_Add(uS_TIMER_SPARK_OFF,Ignition_SparkOff,Lawn_Mower.Spark_Duration,0,1);

}

////////////////////////////
// This function turns off the PWM
// and makes sure the pin is off
///////////////////////////////////
void Ignition_SparkOff(void)
{
	// Turn off the PWM
	PWM_Stop();

	// Shut the pin off
	SPARK_PIN=0;
}

///////////////////////////////////////////
// This function opens the injector
// only if the engine is turning
//////////////////////////////////////////
void Ignition_InjectorOn(void)
{
	// Check the engine RPM
//	if(	Lawn_Mower.RPM_TMR3_Counter_Over_Limit== 0) {
		INJECTOR_PIN = 1;
//	}
}

////////////////////////////////////////
// This function turns off the injector
///////////////////////////////////////
void Ignition_InjectorOff(void)
{
	INJECTOR_PIN = 0;
}

////////////////////////////////////////////
// Draw the basic screen layout and text
/////////////////////////////////////////////
void Draw_Screen(void)
{

	Console_Cursor_Home(COM0);
	Console_Insert_Lines(COM0,10);

	Console_Draw_Box(COM0,1,1,80,10);
	Serial_Send_Full_TX_Buffer(COM0);		// Send all characters in the transmit buffer.

	Console_Cursor_Set(COM0,2,2);
	sprintf(szText,(FARROM*)"Sequence"); // 4 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,2,31);
	sprintf(szText,(FARROM*)"Latency"); // 4 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,3,2);
	sprintf(szText,(FARROM*)"RPM");
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,4,2);
	sprintf(szText,(FARROM*)"Throttle Angle");
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,5,2);
	sprintf(szText,(FARROM*)"Injector Correction"); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,6,2);
	sprintf(szText,(FARROM*)"Injector Duration"); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,7,2);
	sprintf(szText,(FARROM*)"Spark Correction"); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,8,2);
	sprintf(szText,(FARROM*)"Spark Delay"); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,9,2);
	sprintf(szText,(FARROM*)"Spark Length"); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

}

///////////////////////////////////////////////////////
// Put the values where they need to go on
// on the screen
///////////////////////////////////////////////////////
void Compose_Output(void)
{
	
	Console_Attribute(COM0,CONSOLE_ATTR_REVERSE);

	Console_Cursor_Set(COM0,2,22);
	sprintf(szText,(FARROM*)"%8d",Status.Sequence); // 4 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,2,40);
	sprintf(szText,(FARROM*)"%4d",Status.Loop_Latency); // 4 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,3,25);
	sprintf(szText,(FARROM*)"%5d",Lawn_Mower.RPM_Current);
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,4,25);
	sprintf(szText,(FARROM*)"%5d",Lawn_Mower.Throttle_Position);
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,5,25);
	sprintf(szText,(FARROM*)"%5d",Lawn_Mower.Injector_Correction); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);


	Console_Cursor_Set(COM0,6,25);
	sprintf(szText,(FARROM*)"%5d",Lawn_Mower.Injector_Duration); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,7,25);
	sprintf(szText,(FARROM*)"%5d",Lawn_Mower.Spark_Correction); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,8,25);
	sprintf(szText,(FARROM*)"%5d",Lawn_Mower.Spark_Delay); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);

	Console_Cursor_Set(COM0,9,24);
	sprintf(szText,(FARROM*)"%6d",Lawn_Mower.Spark_Duration); // 19 characters
	Serial_Send_String(COM0,szText,SEND_NOW);


	Console_Attribute(COM0,CONSOLE_ATTR_NORMAL);



}


void Process_Serial_Single_Commands()
{
	char c;
	
	c=Serial_GetChar(COM0);
//	if(c==27)
//		 CommandPrompt_TakeCommands();

	if(c== '6') {
		PIN6 ^=1;
	}

	if(c== 'a' || c=='A') {
		DoAbort();
		Abort.User=0;
	}

	if(c=='c' || c=='C') {
		Status.Sequence=0;
		Abort.User=0;
	}

}

void DoAbort()
{


	PIN6=0;

	if(Abort.User) {
		sprintf(szText,(FARROM*)"User Abort\r\n");
		Serial_Send_String(COM0,szText,SEND_NOW);
	}

	
	Compose_Output();	
	
	sprintf(szText,(FARROM*)"Press any key\r\n");
	Serial_Send_String(COM0,szText,SEND_NOW);
	while(!Serial_RX_Data_Waiting(COM0)) {}

}


