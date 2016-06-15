/*! \file eeprom.c
    \brief Internal EEPROM functions
*/

#include "crc.h"
#include "pic.h"
#include "eeprom.h"
#include "interrupts.h"




///////////////////////////////////////////////////////////////////
/// Read data to the EEPROM
/// address = location in eeprom
/// data = pointer to data buffer
/// size = total size of data
/// return (1 for success, 0 for failure)
///////////////////////////////////////////////////////////////////
char EEPROM_Read(unsigned int address, char *data, int size)
{
	unsigned int t;
	if(size > EEPROM_SIZE || (address + size) > EEPROM_SIZE)
		return 0;
	for(t = 0;t < size ; t ++)
	{
		EEADR = address & 0xff;
		EEADRH = address >> 8;
		EECON1bits.EEPGD = 0;
		EECON1bits.CFGS = 0;
		EECON1bits.RD = 1;
		*(char *)(data+t) = EEDATA;
		address++;
	}
	return 1;

}
///////////////////////////////////////////////////////////////////
/// Write data to the EEPROM
/// address = location in eeprom
/// data = pointer to data buffer
/// size = total size of data
/// return (1 for success, 0 for failure)
///////////////////////////////////////////////////////////////////
char EEPROM_Write(int address, char *data, int size)
{
	unsigned int t;
	if(size > EEPROM_SIZE)
		return 0;
 	if((address + size) > EEPROM_SIZE)
		return 0;
	for(t = 0;t < size ; t++)
	{
		EEADR = address & 0xff;
		EEADRH = address >> 8;
		EEDATA = *(char *)(data + t);
		EECON1bits.EEPGD = 0;
		EECON1bits.CFGS = 0;
		EECON1bits.WREN = 1;
		Interrupts_Disable_Global();
		EECON2 = 0x55;
		EECON2 = 0xaa;
		EECON1bits.WR = 1;
		while (EECON1bits.WR);
		Interrupts_Enable_Global();
		address++;
	}

	EECON1bits.WREN = 0;
	return 1;


}

///////////////////////////////////////////////////////////////////
/// Read data to the EEPROM
/// address = location in eeprom
/// data = pointer to data buffer
/// size = total size of data
/// return (1 for success, 0 for failure)
/// The CRC EEPROM functions require four (4) additional
/// bytes.  This needs to be accounted for when storing
/// multiple variables in the EEPROM.
///////////////////////////////////////////////////////////////////
char EEPROM_Read_CRC(unsigned int address, void *data, int size)
{
	unsigned int t;
	unsigned int crc = CRC_CCITT_SEED;
	unsigned int eecrc;
	if(size > EEPROM_SIZE || (address + size) > EEPROM_SIZE)
		return 0;
	for(t = 0;t < size ; t ++)
	{
		EEADR = address & 0xff;
		EEADRH = address >> 8;
		EECON1bits.EEPGD = 0;
		EECON1bits.CFGS = 0;
		EECON1bits.RD = 1;
		*(char *)(data+t) = EEDATA;
		CRC_CCITT_CHAR(EEDATA,&crc);
		address++;
	}
	EEPROM_Read(address, (char *)eecrc,sizeof(eecrc));
	if(eecrc != crc)
		return 0;
	return 1;

}

///////////////////////////////////////////////////////////////////
/// Write data to the EEPROM with a CRC following it
/// address = location in eeprom
/// data = pointer to data buffer
/// size = total size of data
/// return (1 for success, 0 for failure)
/// The CRC EEPROM functions require four (4) additional
/// bytes.  This needs to be accounted for when storing
/// multiple variables in the EEPROM.
///////////////////////////////////////////////////////////////////
char EEPROM_Write_CRC(unsigned int address, void *data, int size)
{
	unsigned int t;
	unsigned int crc = CRC_CCITT_SEED;

	if(size > EEPROM_SIZE || (address + size) > EEPROM_SIZE)
		return 0;

	for(t = 0;t < size ; t++)
	{
		EEADR = address & 0xff;
		EEADRH = address >> 8;
		EEDATA = *(char *)(data + t);
		CRC_CCITT_CHAR(EEDATA,&crc);
		EECON1bits.EEPGD = 0;
		EECON1bits.CFGS = 0;
		EECON1bits.WREN = 1;
		Interrupts_Disable_Global();
		EECON2 = 0x55;
		EECON2 = 0xaa;
		EECON1bits.WR = 1;
		while (EECON1bits.WR);
		Interrupts_Enable_Global();
		address++;
	}
	EEPROM_Write(address,(char *)crc,sizeof(crc));
	EECON1bits.WREN = 0;
	return 1;
}

