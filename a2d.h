#ifndef __A2D_H
#define __A2D_H 

/////////////////////////////////////////////////////////
// ADCON0 Control Register (Page 225)
/////////////////////////////////////////////////////////
#define AN0										1
#define AN1 									5
#define AN2					    				9
#define AN3										13
#define AN4										17

int A2D_GetValue(unsigned char port);
int A2D_GetValue_Average(unsigned char port,char q);
void A2D_Init(void);
int A2D_Read_Result(void);
void A2D_GetValue_Interrupt(unsigned char port);

#endif
