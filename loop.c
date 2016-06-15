/*! \file loop.c
    \brief Main Loop Configuration

	This sets the timing interval for the main loop.
*/

#include "pic.h"
#include "loop.h"
#include "timer.h"
#include "interrupts.h"


void Configure_Loop(int loop);

/////////////////////////////////////////////////////////////
/// Set up initial clock settings to get proper loop speed
/////////////////////////////////////////////////////////////
void Loop_Configure(int hz)
{
 	//////////////////////////////////////////////////////////////
	// Calculate Timer_Interval
	// Cpu Hz = 32,000,000 (HS PLL Enabled)
	// PIC CPU Scaler = 4 (Four cyclers per instruction)
	// Timer Prescaler = 32
	// Frequency = 100hz
	// 32,000,000 / 4		(Divide by CPU ticker per cycle)
	//  8,000,000 / 2 (pre-scaler)	(Divide by prescaler)
	//   4,000,000 / 100hz
	//     40,000
	////////////////////////////////////////////////
	// This routine is not optimized in any way
	// I left it like this to be easier to figure out
	/////////////////////////////////////////////////

	unsigned long t;
	Status.Loop_Frequency = hz;

	t = CLOCK_HZ;	// frequency
	t = t /	4;						// Divide by 4, 4 clock cycles per instruction
	t = t / TIMER0_PRESCALER;		// Divide by timer prescaler
	t = t / hz;

	Status.Loop_Cycles = t;
	Interrupts_Timer0_Disable();

	Timers_Timer0_Enable();	
}
char Loop_Next()
{
	unsigned long v = Timers_Timer0_GetValue();
	if(v > Status.Loop_Cycles) {
		Status.Loop_Latency=v-Status.Loop_Cycles;	// Update Latency
		Timers_Timer0_SetValue(0);
		Status.Sequence++;
		return 1;
	} else {
		return 0;
	}
}


