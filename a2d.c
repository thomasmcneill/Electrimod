#include "pic.h"
#include "a2d.h"
#include "interrupts.h"

void A2D_Init(void)
{
	

	ADCON1bits.VCFG1 = 0;			// Voltage Reference Vss (Ground)
	ADCON1bits.VCFG0 = 0;			// Voltage Reference Vdd (3.3 or 5V)
	ADCON1bits.PCFG3 = 1;			// Configure AN0 - AN4 as analog in	
	ADCON1bits.PCFG2 = 0;
	ADCON1bits.PCFG1 = 1;
	ADCON1bits.PCFG0 = 0;
	ADCON2bits.ADFM = 1;			// Right Justified
	ADCON2bits.ACQT2 = 1; 			// Acquisition time (110) 16 TAD
	ADCON2bits.ACQT1 = 1;
	ADCON2bits.ACQT0 = 0;
	ADCON2bits.ADCS2 = 1;			// AD Clock (Fosc/16)
	ADCON2bits.ADCS1 = 0; 
	ADCON2bits.ADCS0 = 1;
}

int A2D_GetValue_Average(unsigned char port,char q)
{
	char t;
	int val;
	long Average=0;


	for(t=0;t<q;t++) {
		Average+=A2D_GetValue(port);
	}
	val=Average/q;
	return val;
}

int A2D_GetValue(unsigned char port)
{
	int val;
	Interrupts_A2D_Clear();
	ADCON0=port;
	ADCON0bits.GO = 1;
	while (ADCON0bits.NOT_DONE);
	val=ADRESH;
	val <<=8;
	val+=ADRESL;
	return val;
}

void A2D_GetValue_Interrupt(unsigned char port)
{
	Interrupts_A2D_Clear();
	Interrupts_A2D_Enable();
	ADCON0=port;
	ADCON0bits.GO = 1;

}
int A2D_Read_Result(void)
{
	int val;

	val=ADRESH;
	val <<=8;
	val+=ADRESL;
	return val;

}