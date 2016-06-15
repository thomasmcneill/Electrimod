#ifndef __SERVO_H
#define __SERVO_H

#define SERVO_MAX_SERVOS 4

typedef struct  {
	char pin;
	char max_angle;
	char mode;		// 0 == blocking, 1 == Timer_uS
	char uS_index;
	unsigned int angle_delay;
	unsigned int min_pulse;
	unsigned int width;
} SERVO_PARAM;

extern SERVO_PARAM Servo_Params[SERVO_MAX_SERVOS];
extern unsigned int Servo_Update_Interval_uS;
void Servo_Init(int ms, char index);
void Servo_Update_Func(void);
void Servo_SetAngle(char servonum,char angle);
void Servo_Add(char servonum, char pin, char angle,char max_angle,unsigned int min_pulse,unsigned int max_pulse,char uS_Index);
void Servo_Remove(char servonum);
void Servo_Delay(unsigned int angle_delay);
void Servo_Start(void);
void Servo_Stop(void);

#endif
