#ifndef __TIMER_H
#define __TIMER_H 

void Timers_Timer0_SetValue(unsigned int v);
unsigned long Timers_Timer0_GetValue(void);
void Timers_Timer0_Enable(void);



////////////////////////////////////////
// This timer increments every micro second
// functions can be hooked at different intervals
// which allows it to be shared
// be careful not to set it for too low of
// an interval since it will cause the system
// to only run the function designated
// The timer is set based on the least common
// demonitator.  The function has to
////////////////////////////////////////



void Timers_Timer1_uS_Init(void);
void Timers_Timer1_DelayExecute(void (*func)(void),unsigned int uS);
void Timers_Timer1_uS_Queue_Add(char index, void (*func)(void),unsigned int uS, char repeat, char start);
void Timers_Timer1_uS_Queue_Remove(void (*func)(void));
void Timers_Timer1_uS_Queue_Process(void);
void Timers_Timer1_uS_Queue_Start(void);
void Timers_Timer1_uS_Queue_Init(void);
#define TIMERS_TIMER1_US_QUEUE_MAX 6
typedef struct {
	void (*func)(void);
	unsigned int uS;
	char repeat;
	char enabled;
	unsigned int repeat_uS;
} _TIMERS_TIMER1_US_QUEUE;
extern _TIMERS_TIMER1_US_QUEUE Timers_Timer1_uS_Queue[TIMERS_TIMER1_US_QUEUE_MAX];
extern unsigned int  Timers_Timer1_uS_Queue_Current_uS;

unsigned int Timers_Timer1_GetValue(void);
void Timers_Timer3_SetValue(unsigned int v);
unsigned int Timers_Timer3_GetValue(void);
void Timers_Timer3_Enable(char bits, char prescaler);

/*
This is from the MCC18 Documentation

Enable Timer0 Interrupt:
	TIMER_INT_ON Interrupt enabled
	TIMER_INT_OFF Interrupt disabled
Timer Width:
	T0_8BIT 8-bit mode
	T0_16BIT 16-bit mode
Clock Source:
	T0_SOURCE_EXT External clock source (I/O pin)
	T0_SOURCE_INT Internal clock source (TOSC)
External Clock Trigger (for T0_SOURCE_EXT):
	T0_EDGE_FALL External clock on falling edge
	T0_EDGE_RISE External clock on rising edge
Prescale Value:
	T0_PS_1_1 1:1 prescale
	T0_PS_1_2 1:2 prescale
	T0_PS_1_4 1:4 prescale
	T0_PS_1_8 1:8 prescale
	T0_PS_1_16 1:16 prescale
	T0_PS_1_32 1:32 prescale
	T0_PS_1_64 1:64 prescale
	T0_PS_1_128 1:128 prescale
	T0_PS_1_256 1:256 prescale

*/



#endif
