/*! \file crc.c
    \brief crc functions

*/

#include "crc.h"
#include "pic.h"

unsigned int CRC_XModem(far rom char *Data,int length,unsigned int crc_seed);
unsigned int CRC_iButton(far rom char *Data,int length,unsigned int crc_seed);
unsigned int CRC_CCITT(far rom char *Data,int length,unsigned int crc_seed);
unsigned int CRC_16(far rom char *Data,int length,unsigned int crc_seed);

///////////////////////////////////////////////////////////////
/// Computes CRC 16 on ROM data
/// Data = ROM Pointer to data
/// length = total bytes of data
/// crc = Seed value for the CRC
/// returns the CRC
///////////////////////////////////////////////////////////////
unsigned int CRC_16(far rom char *Data,int length,unsigned int crc_seed)
{
    int s,t;
	unsigned int crc = crc_seed;
	for(t=0;t<length;t++)
	{
		crc ^= Data[t];
        for (s=0;s<8;s++)
        {
	    	if (crc & 1)
               crc=(crc >> 1)^0xA001;
            else
                crc=crc >> 1;
        }
	}
	return crc;

}
///////////////////////////////////////////////////////////////
/// Computes CCITT CRC on ROM data
/// Data = ROM Pointer to data
/// length = total bytes of data
/// crc = Seed value for the CRC
/// returns the CRC
///////////////////////////////////////////////////////////////
unsigned int CRC_CCITT(far rom char *Data,int length,unsigned int crc_seed)
{ 

 	
	int t,s;
	char c,d;
	unsigned int crc = crc_seed;	
	for(t=0;t<length;t++)
	{
		c=Data[t];
   		crc  = (unsigned char)(crc >> 8) | (crc << 8);
    	crc ^= c;
    	crc ^= (unsigned char)(crc & 0xff) >> 4;
    	crc ^= (crc<<8) << 4;
    	crc ^= ((crc & 0xff) << 4) << 1;

	}
	return crc;
}

///////////////////////////////////////////////////////////////
/// Computes CCITT CRC one character at a time
/// c = Next character
/// crc = pointer to unsigned int containing crc
///////////////////////////////////////////////////////////////
void CRC_CCITT_CHAR(char c,unsigned int *crc)
{ 

	*crc  = (unsigned char)(*crc >> 8) | (*crc << 8);
    *crc ^= c;
    *crc ^= (unsigned char)(*crc & 0xff) >> 4;
    *crc ^= (*crc<<8) << 4;
    *crc ^= ((*crc & 0xff) << 4) << 1;
}

///////////////////////////////////////////////////////////////
/// Computes iButton CRC ROM data
/// Data = ROM Pointer to data
/// length = total bytes of data
/// crc = Seed value for the CRC
/// returns the CRC
///////////////////////////////////////////////////////////////
unsigned int CRC_iButton(far rom char *Data,int length,unsigned int crc_seed)
{
	int t,s;
	unsigned int crc = crc_seed;
	for(t=0;t<length;t++)
	{
		crc = crc ^ Data[t];
		for(s=0;s<8;s++)
		{
			if(crc & 0x01)
			{
				crc = (crc_seed >> 1);
				crc ^= 0x8c;
			} else {
				crc>>=1;	
			}
		}
	}
	return crc;
}
///////////////////////////////////////////////////////////////
/// Computes XModem CRC for ROM data
/// Data = ROM Pointer to data
/// length = total bytes of data
/// crc = Seed value for the CRC
/// returns the CRC
///////////////////////////////////////////////////////////////
unsigned int CRC_XModem(far rom char *Data,int length,unsigned int crc_seed)
{
	int s,t;
	unsigned int d;
	unsigned int crc = crc_seed;
	for(t=0;t<length;t++)
	{
		d = Data[t];
		crc = crc ^ (d << 8);
		for(s=0;s<8;s++)
		{
			if(crc & 0x8000) 
			{
				crc <<=1;
				crc ^= 0x1021;
			} else {
				crc <<=1;
			}
		}

	}
	return crc;
}