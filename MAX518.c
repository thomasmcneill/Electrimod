#include "i2c.h"
#include "max518.h"

char MAX518_Set(char Channel, unsigned char Value)
{
	int t;
	I2C_Init(100,1000000);
	I2C_Start();
	t=I2C_TX(0b01011000);
	if(t!=0)
		return t;
	t=I2C_TX(Channel);
	if(t!=0)
		return t;
	t=I2C_TX(Value);
	if(t!=0)
		return t;
	I2C_Stop();	


}