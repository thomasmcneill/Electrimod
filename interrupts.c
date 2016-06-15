/*! \file interrupts.c
    \brief Interrupt Control

	Enable and disable interrupts
*/

#include "pic.h"
#include "interrupts.h"
#include "serial.h"


void (*Interrupts_RX_Function[DEVICE_SERIAL_PORTS])(void);
void (*Interrupts_TX_Function[DEVICE_SERIAL_PORTS])(void);
void (*Interrupts_Timer0_Function)(void);
void (*Interrupts_Timer1_Function)(void);
void (*Interrupts_Timer2_Function)(void);
void (*Interrupts_Timer3_Function)(void);
void (*Interrupts_Pin0_Function)(void);
void (*Interrupts_Pin1_Function)(void);
void (*Interrupts_Pin2_Function)(void);
void (*Interrupts_Pin47_Function)(void);
void (*Interrupts_I2CSPI_Function)(void);
void (*Interrupts_A2D_Function)(void);
void (*Interrupts_EEPROM_Function)(void);



void Interrupts_Init(void)
{
	char t;

	for(t=0;t<DEVICE_SERIAL_PORTS;t++)
	{
		Interrupts_RX_Function[t] = 0;
		Interrupts_TX_Function[t] = 0;
	}	
	Interrupts_Timer0_Function = 0;
	Interrupts_Timer1_Function = 0;
	Interrupts_Timer2_Function = 0;
	Interrupts_Timer3_Function = 0;
	Interrupts_Pin0_Function = 0;
	Interrupts_Pin1_Function = 0;
	Interrupts_Pin2_Function = 0;
	Interrupts_Pin47_Function = 0;
	Interrupts_I2CSPI_Function = 0;
	Interrupts_A2D_Function = 0;

	Interrupts_Enable_Peripheral();
	Interrupts_Enable_Global();

}
/////////////////////////////////////////////
/// High priority interrupt vector
/////////////////////////////////////////////
#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
	_asm
   		goto InterruptHandlerHigh //jump to interrupt routine
  	_endasm
}
#pragma code


/////////////////////////////////////////////
/// High priority interrupt vector
/////////////////////////////////////////////
#pragma interrupt InterruptHandlerHigh
void InterruptHandlerHigh ()
{
	char t;
	if(Interrupts_Timer0_Flag()) {
		if( Interrupts_Timer0_Function != 0) {
			Interrupts_Timer0_Function();
			Interrupts_Timer0_Clear();
		}
	}
	if(Interrupts_Timer1_Flag()) {
		if( Interrupts_Timer1_Function != 0) {
			Interrupts_Timer1_Function();
			Interrupts_Timer1_Clear();
		}
	}
	if(Interrupts_Timer2_Flag()) {
		if( Interrupts_Timer2_Function != 0) {
			Interrupts_Timer2_Function();
			Interrupts_Timer2_Clear();
		}
	}
	if(Interrupts_Timer3_Flag()) {
		if( Interrupts_Timer3_Function != 0) {
			Interrupts_Timer3_Function();
			Interrupts_Timer3_Clear();
		}
	}
	if(Interrupts_EEPROM_Flag()) {
		if( Interrupts_EEPROM_Function != 0) {
			Interrupts_EEPROM_Function();
			Interrupts_Timer3_Clear();
		}
	}
	if(Interrupts_Pin0_Flag()) {
		if( Interrupts_Pin0_Function != 0) {
			Interrupts_Pin0_Function();
			Interrupts_Pin0_Clear();
		}
	}
	if(Interrupts_Pin1_Flag()) {
		if( Interrupts_Pin1_Function != 0) {
			Interrupts_Pin1_Function();
			Interrupts_Pin1_Clear();
		}
	}
	if(Interrupts_Pin2_Flag()) {
		if( Interrupts_Pin2_Function != 0) {
			Interrupts_Pin2_Function();
			Interrupts_Pin2_Clear();
		}
	}
	if(Interrupts_Pin47_Flag()) {
		if( Interrupts_Pin47_Function != 0) {
			Interrupts_Pin47_Function();
			Interrupts_Pin47_Clear();
		}
	}
	if(Interrupts_A2D_Flag()) {
		if( Interrupts_A2D_Function != 0) {
			Interrupts_A2D_Function();
			Interrupts_A2D_Clear();
		}
	}
	for(t = 0;t<DEVICE_SERIAL_PORTS;t++)
	{
		if(Interrupts_RX_Flag(t)) {
			if( Interrupts_RX_Function[t] != 0) {
				Interrupts_RX_Function[t]();
			}
		}
		if(Interrupts_TX_Flag(t)) {
			if( Interrupts_TX_Function[t] != 0) {
				Interrupts_TX_Function[t]();
			}
		}
	}	
	if(Interrupts_I2CSPI_Flag()) {
		if( Interrupts_I2CSPI_Function != 0) {
			Interrupts_I2CSPI_Function();
			Interrupts_I2CSPI_Clear();
		}
	}

	
}




//////////////////////////////////////////////////////////////
/// Enable Peripheral Interupts
//////////////////////////////////////////////////////////////
void Interrupts_Enable_Peripheral()
{
	INTCONbits.PEIE = 1;          //peripheral interrupt enable
}

//////////////////////////////////////////////////////////////
/// Enable Global Interupts
//////////////////////////////////////////////////////////////
void Interrupts_Enable_Global()
{
	INTCONbits.GIEH = 1;          //global interrupt enable
}

//////////////////////////////////////////////////////////////
/// Disable Peripheral Interupts
//////////////////////////////////////////////////////////////
void Interrupts_Disable_Peripheral()
{
	INTCONbits.PEIE = 0;          //peripheral interrupt disable
}

//////////////////////////////////////////////////////////////
/// Disable Global Interupts
//////////////////////////////////////////////////////////////
void Interrupts_Disable_Global()
{
	INTCONbits.GIEH = 0;          //global interrupt disable
}


//////////////////////////////////////////////////////////////
/// Disable Timer0 Overflow Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Timer0_Disable()
{
	INTCONbits.TMR0IE = 0;         
	
}
//////////////////////////////////////////////////////////////
/// Enable Timer0 Overflow Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Enable_Timer0()
{
	INTCONbits.TMR0IE = 1;     
	INTCON2bits.TMR0IP = 1;    
}





//////////////////////////////////////////////////////////////
/// Disable Pin0 External Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Pin0_Disable()
{
	INTCONbits.INT0IE = 0;          
}
//////////////////////////////////////////////////////////////
/// Enable INT0 External Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Pin0_Enable()
{
	INTCONbits.INT0IE = 1;          
	
}

//////////////////////////////////////////////////////////////
/// Disable Pin1 External Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Pin1_Disable()
{
	INTCON3bits.INT1IE = 0;          
}
//////////////////////////////////////////////////////////////
/// Enable Pin1 External Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Pin1_Enable()
{
	INTCON3bits.INT1IE = 1;          
	INTCON3bits.INT1IP = 1;
}

//////////////////////////////////////////////////////////////
/// Disable Pin2 External Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Pin2_Disable()
{
	INTCON3bits.INT2IE = 0;          
}
//////////////////////////////////////////////////////////////
/// Enable Pin2 External Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Pin2_Enable()
{
	INTCON3bits.INT2IE = 1;          
	INTCON3bits.INT2IP = 1;
}


//////////////////////////////////////////////////////////////
/// Disable Pins 4 through 7 External Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Pins47_Disable(void)
{
	INTCONbits.RBIE = 0; 
}
//////////////////////////////////////////////////////////////
/// Enable Pins 4 through 7 External Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Pins47_Enable(void)
{
	INTCONbits.RBIE = 1; 
	INTCON2bits.RBIP = 1;
}

//////////////////////////////////////////////////////////////
/// Port 0 triggers on rising edge
//////////////////////////////////////////////////////////////
void Interrupts_Pin0_Rising()
{
	INTCON2bits.INTEDG0 = 1;
}

//////////////////////////////////////////////////////////////
/// Port 0 triggers on falling
//////////////////////////////////////////////////////////////
void Interrupts_Pin0_Falling()
{
	INTCON2bits.INTEDG0 = 0;
}

//////////////////////////////////////////////////////////////
/// Port 1 triggers on rising edge
//////////////////////////////////////////////////////////////
void Interrupts_Pin1_Rising()
{
	INTCON2bits.INTEDG1 = 1;
}

//////////////////////////////////////////////////////////////
/// Port 1 triggers on falling
//////////////////////////////////////////////////////////////
void Interrupts_Pin1_Falling()
{
	INTCON2bits.INTEDG1 = 0;
}


//////////////////////////////////////////////////////////////
/// Port 2 triggers on rising edge
//////////////////////////////////////////////////////////////
void Interrupts_Pin2_Rising()
{
	INTCON2bits.INTEDG2 = 1;
}

//////////////////////////////////////////////////////////////
/// Port 2 triggers on falling
//////////////////////////////////////////////////////////////
void Interrupts_Pin2_Falling()
{
	INTCON2bits.INTEDG2 = 0;
}


//////////////////////////////////////////////////////////////
/// Check Timer 0 Interrupt Occured
//////////////////////////////////////////////////////////////
char Interrupts_Timer0_Flag()
{
	return INTCONbits.TMR0IF;
}
//////////////////////////////////////////////////////////////
/// Clear Timer 0 Interrupt Flag
//////////////////////////////////////////////////////////////
void Interrupts_Timer0_Clear()
{
	INTCONbits.TMR0IF = 0;
}
//////////////////////////////////////////////////////////////
/// Check Timer 1 Interrupt Occured
//////////////////////////////////////////////////////////////
char Interrupts_Timer1_Flag()
{
	return PIR1bits.TMR1IF;
}
//////////////////////////////////////////////////////////////
/// Clear Timer 1 Interrupt Flag
//////////////////////////////////////////////////////////////
void Interrupts_Timer1_Clear()
{
	PIR1bits.TMR1IF= 0;
}

//////////////////////////////////////////////////////////////
/// Check Timer 2 Interrupt Occured
//////////////////////////////////////////////////////////////
char Interrupts_Timer2_Flag()
{
	return PIR1bits.TMR2IF;
}
//////////////////////////////////////////////////////////////
/// Clear Timer 2 Interrupt Flag
//////////////////////////////////////////////////////////////
void Interrupts_Timer2_Clear()
{
	PIR1bits.TMR2IF = 0;
}

//////////////////////////////////////////////////////////////
/// Check Timer 3 Interrupt Occured
//////////////////////////////////////////////////////////////
char Interrupts_Timer3_Flag()
{
	return PIR2bits.TMR3IF;
}
//////////////////////////////////////////////////////////////
/// Clear Timer 3 Interrupt Flag
//////////////////////////////////////////////////////////////
void Interrupts_Timer3_Clear()
{
	PIR2bits.TMR3IF = 0;
}

//////////////////////////////////////////////////////////////
/// Check EEPROM Status
//////////////////////////////////////////////////////////////
char Interrupts_EEPROM_Flag()
{
	return PIR2bits.EEIF;
}
//////////////////////////////////////////////////////////////
/// Clear EEPROM Status
//////////////////////////////////////////////////////////////
void Interrupts_EEPROM_Clear()
{
	PIR2bits.EEIF = 0;
}
//////////////////////////////////////////////////////////////
/// Wait for EEPROM and clear it
//////////////////////////////////////////////////////////////
void Interrupts_EEPROM_Wait()
{
	while(PIR2bits.EEIF == 0);
	PIR2bits.EEIF = 0;
}



//////////////////////////////////////////////////////////////
/// Check Port 0 Interrupt Occured
//////////////////////////////////////////////////////////////
char Interrupts_Pin0_Flag()
{
	return INTCONbits.INT0IF;
}


//////////////////////////////////////////////////////////////
/// Clear Port 0 Interrupt Flag
//////////////////////////////////////////////////////////////
void Interrupts_Pin0_Clear()
{
	INTCONbits.INT0IF = 0;
}


//////////////////////////////////////////////////////////////
/// Check Port 4 - 7 Interrupt Occured
//////////////////////////////////////////////////////////////
char Interrupts_Pin47_Flag()
{
	return INTCONbits.RBIF;
}


//////////////////////////////////////////////////////////////
/// Clear Port 4 - 7 Interrupt Flag
//////////////////////////////////////////////////////////////
void Interrupts_Pin47_Clear()
{
	INTCONbits.RBIF = 0;
}


//////////////////////////////////////////////////////////////
/// Check Port 1 Interrupt Occured
//////////////////////////////////////////////////////////////
char Interrupts_Pin1_Flag()
{
	return INTCON3bits.INT1IF;
}


//////////////////////////////////////////////////////////////
/// Clear Port 1 Interrupt Flag
//////////////////////////////////////////////////////////////
void Interrupts_Pin1_Clear()
{
	INTCON3bits.INT1IF = 0;
}

//////////////////////////////////////////////////////////////
/// Check Port 2 Interrupt Occured
//////////////////////////////////////////////////////////////
char Interrupts_Pin2_Flag()
{
	return INTCON3bits.INT2IF;
}


//////////////////////////////////////////////////////////////
/// Clear Port 2 Interrupt Flag
//////////////////////////////////////////////////////////////
void Interrupts_Pin2_Clear()
{
	INTCON3bits.INT2IF = 0;
}


//////////////////////////////////////////////////////////////
/// Check Analog Conversion Status
//////////////////////////////////////////////////////////////
char Interrupts_A2D_Flag()
{
	return PIR1bits.ADIF;
}


//////////////////////////////////////////////////////////////
/// Clear Analog Conversion Flag
//////////////////////////////////////////////////////////////
void Interrupts_A2D_Clear()
{
	PIR1bits.ADIF = 0;
}

//////////////////////////////////////////////////////////////
/// Check RX Interrupt Occured.  Returns 1 if the buffer has a character
//////////////////////////////////////////////////////////////
char Interrupts_RX_Flag(char port)
{
	if(port == 0)
		return PIR1bits.RCIF;
	else 
		return 0;
}


//////////////////////////////////////////////////////////////
/// Enable RX Interrupts
//////////////////////////////////////////////////////////////
void Interrupts_RX_Enable(char port)
{
	if(port == 0) {
		PIE1bits.RCIE= 1;
		IPR1bits.RCIP = 1;
	}
}
//////////////////////////////////////////////////////////////
/// Disable RX Interrupts
//////////////////////////////////////////////////////////////
void Interrupts_RX_Disable(char port)
{
	if(port == 0)
		PIE1bits.RCIE= 0;
}
//////////////////////////////////////////////////////////////
/// Enable TX Interrupts
//////////////////////////////////////////////////////////////
void Interrupts_TX_Enable(char port)
{
	if(port == 0)
	{
		PIE1bits.TXIE= 1;
		IPR1bits.TXIP = 1;
	}

}
//////////////////////////////////////////////////////////////
/// Disable TX Interrupts
//////////////////////////////////////////////////////////////
void Interrupts_TX_Disable(char port)
{
	if(port == 0)
		PIE1bits.TXIE= 0;
}

//////////////////////////////////////////////////////////////
/// Check TX Status.  Returns 1 if it can be written too.
//////////////////////////////////////////////////////////////
char Interrupts_TX_Flag(char port)
{
	if(port == 0)
		return PIR1bits.TXIF;
	else
		return 0;
}



//////////////////////////////////////////////////////////////
/// Check I2C/SPI Status returns if rx/tx is complete
//////////////////////////////////////////////////////////////
char Interrupts_I2CSPI_Flag()
{
	return PIR1bits.SSPIF;
}
//////////////////////////////////////////////////////////////
/// Eanble I2CSPI Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_I2CSPI_Enable()
{
	PIE1bits.SSPIE = 1;
	IPR1bits.SSPIP = 1;
}
//////////////////////////////////////////////////////////////
/// disable I2CSPI Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_I2CSPI_Disable()
{
	PIE1bits.SSPIE = 0;
}

//////////////////////////////////////////////////////////////
/// Clear I2C/SPI Status 
//////////////////////////////////////////////////////////////
void Interrupts_I2CSPI_Clear()
{
	PIR1bits.SSPIF = 0;
}


//////////////////////////////////////////////////////////////
/// Enable Timer 2 Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Timer2_Enable()
{
	PIE1bits.TMR2IE = 1;
	IPR1bits.TMR2IP = 1;
}
//////////////////////////////////////////////////////////////
/// disable Timer 2 Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Timer2_Disable()
{
	PIE1bits.TMR2IE = 0;
}

//////////////////////////////////////////////////////////////
/// Enable Timer 1 Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Timer1_Enable()
{
	PIE1bits.TMR1IE = 1;
	IPR1bits.TMR1IP = 1;
}
//////////////////////////////////////////////////////////////
/// disable Timer 1 Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Timer1_Disable()
{
	PIE1bits.TMR1IE = 0;
}


//////////////////////////////////////////////////////////////
/// Enable A2D Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_A2D_Enable()
{
	PIE1bits.ADIE = 1;
	IPR1bits.ADIP = 1;
}
//////////////////////////////////////////////////////////////
/// disable A2D Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_A2D_Disable()
{
	PIE1bits.ADIE = 0;
}

//////////////////////////////////////////////////////////////
/// Enable Timer 3 Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Timer3_Enable()
{
	PIE2bits.TMR3IE = 1;
	IPR2bits.TMR3IP = 1;
}
//////////////////////////////////////////////////////////////
/// disable Timer 3 Interrupt
//////////////////////////////////////////////////////////////
void Interrupts_Timer3_Disable()
{
	PIE2bits.TMR3IE = 0;
}















