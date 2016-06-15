#include "i2c.h"
#include "max518.h"

void MAX518_Set(char Channel, unsigned char Value)
{
	I2C_Init(100,10000);
	I2C_Start();
	I2C_TX(0b01011000);
	I2C_TX(Channel);
	I2C_TX(Value);
	I2C_Stop();	


}