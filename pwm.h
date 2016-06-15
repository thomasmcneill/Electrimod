/*! \file pwm.h
    \brief PWM Functions
*/

// Standard Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


#ifndef __pwm_H
#define __pwm_H 

char PWM_Configure(char Channel,unsigned long Frequency, char Duty);
extern unsigned char PWM_CCP2CON;
extern unsigned char PWM_PR2;
extern unsigned char PWM_T2CON;
extern unsigned char PWM_CCPR2L;
extern unsigned char PWM_CCP2CON;

void PWM_Start();
void PWM_Stop();

#endif
