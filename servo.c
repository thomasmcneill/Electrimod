#include "servo.h"
#include "pic.h"
#include "timer.h"

SERVO_PARAM Servo_Params[SERVO_MAX_SERVOS];
unsigned int Servo_Update_Interval_uS;

void Servo_Init(int ms,char index)
{
	char t;
	Servo_Update_Interval_uS=ms*1000;
	
	for(t = 0;t<SERVO_MAX_SERVOS;t++) 
	{
		Servo_Params[t].pin = -1;
	}
	if(index != -1)
		Timers_Timer1_uS_Queue_Add(index,Servo_Update_Func,Servo_Update_Interval_uS,1,1);

}

void Servo_Update_Func(void)
{
	unsigned int angle_delay;
	char mode;
	char uS_index;
	char pin;
	char t;

	t=0;
	do {

		pin = Servo_Params[t].pin;
		uS_index = Servo_Params[t].uS_index;
		angle_delay = Servo_Params[t].angle_delay;

		if(pin == 0)
		{
				PIN0_ON();
				Servo_Delay(angle_delay);
				PIN0_OFF();
		}
		else if(pin == 1)
		{
				PIN1_ON();
				Servo_Delay(angle_delay);
				PIN1_OFF();
		}
		else if(pin == 2)
		{
				PIN2_ON();
				Servo_Delay(angle_delay);
				PIN2_OFF();
		}
		else if(pin == 3)
		{
				PIN3_ON();
				Servo_Delay(angle_delay);
				PIN3_OFF();
		}
		else if(pin == 4)
		{
				PIN4_ON();
				Servo_Delay(angle_delay);
				PIN4_OFF();
		}
		else if(pin == 5)
		{
				if(uS_index == 127) {
					PIN5_ON();
					Timers_Timer1_DelayExecute(PIN5_OFF,angle_delay);
				} else 	if(uS_index == -1) {
					PIN5_ON();
					Servo_Delay(angle_delay);
					PIN5_OFF();
				} else {			
					PIN5_ON();
					Timers_Timer1_uS_Queue_Add(uS_index,PIN5_OFF,angle_delay,0,1);
				}

		}
		else if(pin == 6)
		{
				PIN6_ON();
				Servo_Delay(angle_delay);
				PIN6_OFF();
		}
		t++;
	} while (t!=SERVO_MAX_SERVOS);

}
void Servo_Delay(unsigned int angle_delay)
{
	unsigned int t;
	for(t=0;t<angle_delay;t++) {
		
	}
}
void Servo_SetAngle(char servonum,char angle)
{
	unsigned long angle_delay;

	if(servonum >= 0 && servonum < SERVO_MAX_SERVOS)
	{
		
		angle_delay = Servo_Params[servonum].width;
		angle_delay = angle_delay * angle;
		angle_delay = angle_delay / Servo_Params[servonum].max_angle;
		angle_delay = Servo_Params[servonum].min_pulse + angle_delay;
		if(	Servo_Params[servonum].uS_index == -1) {

			angle_delay = angle_delay * 8; 		// 8 instructions in a uS
			angle_delay = angle_delay - 32;		// about 32 instructions for jmps and what not
			angle_delay = angle_delay / 20;		// for next loop uses about 20 instructions per interation

		}
		Servo_Params[servonum].angle_delay = angle_delay;

	}

}
void Servo_Add(char servonum, char pin, char angle,char max_angle,unsigned int min_pulse,unsigned int max_pulse,char uS_Index)
{
	if(servonum >= 0 && servonum < SERVO_MAX_SERVOS)
	{
		Servo_Params[servonum].pin = pin;
		Servo_Params[servonum].min_pulse = min_pulse;
		Servo_Params[servonum].width =max_pulse - min_pulse;
 		Servo_Params[servonum].max_angle = max_angle;
 		Servo_Params[servonum].uS_index = uS_Index;

		Servo_SetAngle(servonum,angle);
	}
}

void Servo_Remove(char servonum)
{
	if(servonum >= 0 && servonum < SERVO_MAX_SERVOS)
	{
		Servo_Params[servonum].pin = -1;
	}

}
