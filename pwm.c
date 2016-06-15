/*! \file pwm.c
    \brief PWM Functions

	PWM Functions
*/

#include "pic.h"
#include "pwm.h"
#include "timer.h"
#include "interrupts.h"

#include <p18F2525.h>
unsigned char PWM_CCP2CON;
unsigned char PWM_PR2;
unsigned char PWM_T2CON;
unsigned char PWM_CCPR2L;
unsigned char PWM_CCP2CON;
void PWM_Start(void)
{
	CCPR2L = PWM_CCPR2L;
}
void PWM_Stop(void)
{
	CCPR2L = 0;

}

char PWM_Configure(char Channel, unsigned long Frequency, char Duty)
{
	char Prescale = 1;
	unsigned long t,s;
	unsigned char u;

		TRISBbits.RB3 = 0;
	
		do 
		{	
			//  (OSC / (Freq * Prescale)) - 1 = PR2
			t = CLOCK_HZ / Frequency;
			t = t / 4;
			t = t / Prescale;
			t = t - 1;
			
			if(t < 256)
				break;
				
			Prescale = Prescale * 4;
			if(Prescale > 16)	// We couldn't find a working parameters
				return 1;

		} while (1==1);
		PWM_PR2 = t;
		


		if(Prescale == 1)		
			PWM_T2CON = 0b00000000;
		if(Prescale == 4)
			PWM_T2CON = 0b00000001;
		if(Prescale == 16)
			PWM_T2CON = 0b00000011;

		// Duty * OSC / (Frequency * Prescale*100) = CCPR


		t = PWM_PR2;
		t++;
		t = t * 4;
		t = t * Duty;
		t = t / 100;
		s = t >> 2;
		PWM_CCPR2L = s;
		
		PWM_CCP2CON	 =  0b00001100;

		PORTBbits.RB3 = 0;
		TMR2=0;
		CCP2CON =  PWM_CCP2CON;
		PR2 = PWM_PR2;
		T2CON =  PWM_T2CON;
		CCPR2L = 0;
		T2CONbits.TMR2ON = 1;

		
	return 0;	

}
	