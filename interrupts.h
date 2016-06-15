/*! \file interrupts.h
    \brief Interrupt Control Header

	Enable and disable interrupts
*/
#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H


extern void (*Interrupts_RX_Function[DEVICE_SERIAL_PORTS])(void);
extern void (*Interrupts_TX_Function[DEVICE_SERIAL_PORTS])(void);
extern void (*Interrupts_Timer0_Function)(void);
extern void (*Interrupts_Timer1_Function)(void);
extern void (*Interrupts_Timer2_Function)(void);
extern void (*Interrupts_Timer3_Function)(void);
extern void (*Interrupts_Pin0_Function)(void);
extern void (*Interrupts_Pin1_Function)(void);
extern void (*Interrupts_Pin2_Function)(void);
extern void (*Interrupts_Pin47_Function)(void);
extern void (*Interrupts_I2CSPI_Function)(void);
extern void (*Interrupts_A2D_Function)(void);
extern void (*Interrupts_EEPROM_Function)(void);


void InterruptHandlerHigh (void);

void Interrupts_Init(void);
void Interrupts_Enable_Peripheral(void);
void Interrupts_Enable_Global(void);
void Interrupts_Disable_Peripheral(void);
void Interrupts_Disable_Global(void);
void Interrupts_Timer0_Enable(void);
void Interrupts_Timer0_Disable(void);
void Interrupts_Pin0_Disable(void);
void Interrupts_Pin0_Enable(void);
void Interrupts_Pin1_Disable(void);
void Interrupts_Pin1_Enable(void);
void Interrupts_Pin2_Disable(void);
void Interrupts_Pin2_Enable(void);
void Interrupts_Pins47_Disable(void);
void Interrupts_Pins47_Enable(void);

void Interrupts_RX_Enable(char port);
void Interrupts_RX_Disable(char port);
void Interrupts_TX_Enable(char port);
void Interrupts_TX_Disable(char port);

void Interrupts_I2CSPI_Enable(void);
void Interrupts_I2CSPI_Disable(void);
void Interrupts_A2D_Enable(void);
void Interrupts_Timer2_Enable(void);
void Interrupts_Timer2_Disable(void);
void Interrupts_Timer1_Enable(void);
void Interrupts_Timer1_Disable(void);
void Interrupts_Timer3_Enable(void);
void Interrupts_Timer3_Disable(void);


void Interrupts_Pin0_Rising(void);
void Interrupts_Pin0_Falling(void);
void Interrupts_Pin1_Rising(void);
void Interrupts_Pin1_Falling(void);
void Interrupts_Pin2_Rising(void);
void Interrupts_Pin2_Falling(void);

char Interrupts_Timer0_Flag(void);
void Interrupts_Timer0_Clear(void);
char Interrupts_Timer1_Flag(void);
void Interrupts_Timer1_Clear(void);
char Interrupts_Timer2_Flag(void);
void Interrupts_Timer2_Clear(void);
char Interrupts_Timer3_Flag(void);
void Interrupts_Timer3_Clear(void);
char Interrupts_EEPROM_Flag(void);
void Interrupts_EEPROM_Clear(void);
char Interrupts_EEPROM_Flag(void);
void Interrupts_EEPROM_Wait(void);
char Interrupts_Pin0_Flag(void);
void Interrupts_Pin0_Clear(void);
char Interrupts_Pin47_Flag(void);
void Interrupts_Pin47_Clear(void);
char Interrupts_Pin1_Flag(void);
void Interrupts_Pin1_Clear(void);
char Interrupts_Pin2_Flag(void);
void Interrupts_Pin2_Clear(void);
char Interrupts_A2D_Flag(void);
void Interrupts_A2D_Clear(void);
char Interrupts_RX_Flag(char port);
char Interrupts_TX_Flag(char port);
char Interrupts_I2CSPI_Flag(void);
void Interrupts_I2CSPI_Clear(void);




#endif
