/*! \file timer.c
    \brief Timer Control

	These functions provide access to the
	timer.
*/

#include "pic.h"
#include "interrupts.h"
#include "timer.h"

_TIMERS_TIMER1_US_QUEUE Timers_Timer1_uS_Queue[TIMERS_TIMER1_US_QUEUE_MAX];
unsigned int  Timers_Timer1_uS_Queue_Current_uS;
void Timers_Timer1_uS_Queue_Add(char index, void (*func)(void),unsigned int S, char repeat, char start)
{
	char t,s;
	unsigned int timer;
	unsigned int temp;
	unsigned int *uS;
	char tmr_status;

	if(T1CONbits.TMR1ON) {
		tmr_status = 1;			
		T1CONbits.TMR1ON=0;
		timer = TMR1L;
		temp = TMR1H;
		temp = temp << 8;
		timer +=temp;
		timer =  timer - Timers_Timer1_uS_Queue_Current_uS;
	} else {
		tmr_status=0;
		timer = 0;
	}

	
	Timers_Timer1_uS_Queue[index].enabled=1;
	Timers_Timer1_uS_Queue[index].func=func;
	Timers_Timer1_uS_Queue[index].uS=S+timer;
	Timers_Timer1_uS_Queue[index].repeat=repeat;
	Timers_Timer1_uS_Queue[index].repeat_uS=S;

	Timers_Timer1_uS_Queue_Current_uS = 65535;
	t = 0;
	do {
		if(Timers_Timer1_uS_Queue[t].enabled == 1) {
			uS = &Timers_Timer1_uS_Queue[t].uS;
			if(timer < *uS)
			*uS-=timer;
			else 
				*uS = 0;
			if(*uS < Timers_Timer1_uS_Queue_Current_uS) {
				Timers_Timer1_uS_Queue_Current_uS = *uS;
			}
		}
		t++;
	} while(t != TIMERS_TIMER1_US_QUEUE_MAX);
	Timers_Timer1_uS_Queue_Current_uS = 65535 - Timers_Timer1_uS_Queue_Current_uS;
	TMR1H = (Timers_Timer1_uS_Queue_Current_uS >> 8);
	TMR1L = (Timers_Timer1_uS_Queue_Current_uS & 0xff); 

	T1CONbits.TMR1ON = tmr_status; 
	if(start)
		T1CONbits.TMR1ON=1;


}
void Timers_Timer1_uS_Queue_Init(void)
{
	char t;	
	for(t=0;t<TIMERS_TIMER1_US_QUEUE_MAX;t++)
	{
		Timers_Timer1_uS_Queue[t].uS=65535;
		Timers_Timer1_uS_Queue[t].func=0;
		Timers_Timer1_uS_Queue[t].enabled = 0;
			
	}
	Timers_Timer1_uS_Init();
	Timers_Timer1_uS_Queue_Current_uS=65535;
}
void Timers_Timer1_uS_Queue_Start(void)
{
	char t;	
	unsigned int timer;
	Interrupts_Timer1_Function=Timers_Timer1_uS_Queue_Process;
	Timers_Timer1_uS_Queue_Current_uS = 65535;
	t = 0;
	do {
		if(Timers_Timer1_uS_Queue[t].enabled == 1) {
			if(Timers_Timer1_uS_Queue[t].uS < Timers_Timer1_uS_Queue_Current_uS) {
				Timers_Timer1_uS_Queue_Current_uS = Timers_Timer1_uS_Queue[t].uS;
			}
		}
		t++;
	} while(t != TIMERS_TIMER1_US_QUEUE_MAX);
	if(Timers_Timer1_uS_Queue_Current_uS != 65535) {
		timer = 65535 - Timers_Timer1_uS_Queue_Current_uS;
		TMR1H = (timer >> 8);
		TMR1L = (timer & 0xff); 
		PIR1bits.TMR1IF = 0;
		T1CONbits.TMR1ON = 1; 
	}


}
void Timers_Timer1_uS_Queue_Process(void)
{
	unsigned int timer;
	char t;
	unsigned int *uS;
	timer = Timers_Timer1_uS_Queue_Current_uS;
	Timers_Timer1_uS_Queue_Current_uS=65535;

	t = 0;
	do {
		if(Timers_Timer1_uS_Queue[t].enabled) {
			uS = &Timers_Timer1_uS_Queue[t].uS;
			*uS-=timer;

			if(*uS <= 0) {

				Timers_Timer1_uS_Queue[t].func();

				if(Timers_Timer1_uS_Queue[t].repeat) {
					*uS = Timers_Timer1_uS_Queue[t].repeat_uS;
				} else {
					Timers_Timer1_uS_Queue[t].enabled = 0;
					Timers_Timer1_uS_Queue[t].uS = 65535;

				}
			}
			if(*uS < Timers_Timer1_uS_Queue_Current_uS) {
				Timers_Timer1_uS_Queue_Current_uS = *uS;
			}
		}
		t++;
	} while(t != TIMERS_TIMER1_US_QUEUE_MAX);

	if(Timers_Timer1_uS_Queue_Current_uS!=65535) {
 		Timers_Timer1_uS_Queue_Current_uS = 65535 -  Timers_Timer1_uS_Queue_Current_uS;
		TMR1H = (Timers_Timer1_uS_Queue_Current_uS >> 8);
		TMR1L = (Timers_Timer1_uS_Queue_Current_uS & 0xff); 
		T1CONbits.TMR1ON = 1; 
	} else {
		T1CONbits.TMR1ON=0;
	}

	

}
///////////////////////////////////////////////////////////////
/// Enable Timer0.  Timer0 is used for the main loop.
/// 16 Bit Counter, 32 Prescaler.
///////////////////////////////////////////////////////////////
void Timers_Timer0_Enable(void)
{
		int prescaler = TIMER0_PRESCALER;
		/////////////////////////////////////////////////////////////////////////////
		// Set up timer 0 and Timeout_Cycles for 100hz
		/////////////////////////////////////////////////////////////////////////////
		TMR0H = 0;				// Clear initial value
		TMR0L = 0; 
		T0CON = 0;				// Set up intitial timer settings
		T0CONbits.T08BIT = 0;		// 16 bit counter
		switch (prescaler) {
			case 2:
				T0CONbits.T0PS2 = 0; T0CONbits.T0PS1 = 0; T0CONbits.T0PS0 = 0;
				break;
			case 4:
				T0CONbits.T0PS2 = 0; T0CONbits.T0PS1 = 0; T0CONbits.T0PS0 = 1;
				break;
			case 8:
				T0CONbits.T0PS2 = 0; T0CONbits.T0PS1 = 1; T0CONbits.T0PS0 = 0;
				break;
			case 16:
				T0CONbits.T0PS2 = 0; T0CONbits.T0PS1 = 1; T0CONbits.T0PS0 = 1;
				break;
			case 32:
				T0CONbits.T0PS2 = 1; T0CONbits.T0PS1 = 0; T0CONbits.T0PS0 = 0;
				break;
			case 64:
				T0CONbits.T0PS2 = 1; T0CONbits.T0PS1 = 0; T0CONbits.T0PS0 = 1;
				break;
			case 128:
				T0CONbits.T0PS2 = 1; T0CONbits.T0PS1 = 1; T0CONbits.T0PS0 = 0;
				break;
			case 256:
				T0CONbits.T0PS2 = 1; T0CONbits.T0PS1 = 1; T0CONbits.T0PS0 = 1;
				break;
		
		}
		
		T0CONbits.TMR0ON = 1; 		// Enable Timer
}
//////////////////////////////////////////////////////////////
/// Disable timer 0
//////////////////////////////////////////////////////////////
void Timers_Timer0_Disable(void)
{
	T0CONbits.TMR0ON = 0;
}
//////////////////////////////////////////////////////////////
/// Set the timers value
//////////////////////////////////////////////////////////////
void Timers_Timer0_SetValue(unsigned int v)
{
	TMR0H = (v >> 8);
	TMR0L = (v & 0xff); 
}

//////////////////////////////////////////////////////////////
/// Get the timers value
//////////////////////////////////////////////////////////////
unsigned long Timers_Timer0_GetValue(void)
{
	unsigned int tmr0l;
	unsigned long value;
	tmr0l = TMR0L;
	value = TMR0H;
	value = value << 8;
	value = value + tmr0l;
	return value;
}


///////////////////////////////////////////////////////////////
/// Start us Timer 
/// 
///////////////////////////////////////////////////////////////
void Timers_Timer1_uS_Init(void)
{
	char t;


	TMR1H = 0;				// Clear initial value
	TMR1L = 0; 
	T1CON = 0;				// Set up intitial timer settings
	T1CONbits.RD16 = 1;
	T1CONbits.T1CKPS1 = 1; 	
	T1CONbits.T1CKPS0 = 1;

	Interrupts_Timer1_Enable();
	T1CONbits.TMR1ON = 0; 		// Enable Timer
}
void Timers_Timer1_DelayExecute(void (*func)(void),unsigned int uS)
{
	if(T1CONbits.TMR1ON==0) {
	
		uS = 65535 - uS;
		TMR1H = uS >> 8;
		TMR1L = uS & 0xFF; 
		Interrupts_Timer1_Function=func;
		T1CONbits.TMR1ON = 1;
		PIR1bits.TMR1IF = 0;
	}


}
//////////////////////////////////////////////////////////////
/// Stop  timer 1
//////////////////////////////////////////////////////////////
void Timers_Timer1_Stop(void)
{
	T1CONbits.TMR1ON = 0;
}
//////////////////////////////////////////////////////////////
/// Start  timer 1
//////////////////////////////////////////////////////////////
void Timers_Timer1_Start(void)
{
	T1CONbits.TMR1ON = 1;
}

//////////////////////////////////////////////////////////////
/// Set the timers value
//////////////////////////////////////////////////////////////
void Timers_Timer1_SetValue(unsigned int v)
{
	TMR1H = (v >> 8);
	TMR1L = (v & 0xff); 
}

//////////////////////////////////////////////////////////////
/// Get the timers value
//////////////////////////////////////////////////////////////
unsigned int Timers_Timer1_GetValue(void)
{
	unsigned int tmr;
	unsigned long value;
	if(		T1CONbits.RD16 == 0) 
	{
		value = TMR1L;
		return value;
	
	}

	tmr = TMR1L;
	value = TMR1H;
	value = value << 8;
	value = value + tmr;
	return value;
}

///////////////////////////////////////////////////////////////
/// Enable Timer3.  
/// 
///////////////////////////////////////////////////////////////
void Timers_Timer3_Enable(char bits, char prescaler)
{

		TMR3H = 0;				// Clear initial value
		TMR3L = 0; 
		T3CON = 0;				// Set up intitial timer settings
		if(bits)
			T3CONbits.RD16 = 1;
		T3CONbits.T3CCP2 = 1;
		T3CONbits.T3CCP1 = 1;
		switch (prescaler) {
			case 1:
				T3CONbits.T3CKPS1 = 0; 	T3CONbits.T3CKPS0 = 0;
				break;
			case 2:
				T3CONbits.T3CKPS1 = 0; 	T3CONbits.T3CKPS0 = 1;
				break;
			case 4:
				T3CONbits.T3CKPS1 = 1; 	T3CONbits.T3CKPS0 = 0;
				break;
			case 8:
				T3CONbits.T3CKPS1 = 1; 	T3CONbits.T3CKPS0 = 1;
				break;
		
		}
	
			
		T3CONbits.TMR3ON = 1; 		// Enable Timer

}
//////////////////////////////////////////////////////////////
/// Disable timer 3
//////////////////////////////////////////////////////////////
void Timers_Timer3_Disable(void)
{
	T3CONbits.TMR3ON = 0;
}
//////////////////////////////////////////////////////////////
/// Set the timers value
//////////////////////////////////////////////////////////////
void Timers_Timer3_SetValue(unsigned int v)
{
	TMR3H = (v >> 8);
	TMR3L = (v & 0xff); 
}

//////////////////////////////////////////////////////////////
/// Get the timers value
//////////////////////////////////////////////////////////////
unsigned int Timers_Timer3_GetValue(void)
{
	unsigned int tmr;
	unsigned long value;
	if(		T3CONbits.RD16 == 0) 
	{
		value = TMR3L;
		return value;
	
	}
	tmr = TMR3L;
	value = TMR3H;
	value = value << 8;
	value = value + tmr;
	return value;
}




