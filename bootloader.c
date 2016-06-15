/*! \file bootloader.c
    \brief Boot Loader Functions

	These functions provide access to updating
	the bootloader and invoking it.
*/

#include "pic.h"
#include "bootloader.h"
#include "18f2525_boot.h"
#include "serial.h"
#include "crc.h"
#include "commandprompt.h"
#include "interrupts.h"


far rom char *ROMBootLoader;
void BootLoaderInit();
void BootLoader_Update_ROM();
void BootLoader_Start();


void BootLoader_Start()
{
	_asm
		call  Boot_Loader_Start,0
	_endasm

}
void BootLoaderInit()
{
	unsigned int LoaderLength = sizeof(BootLoader);
	unsigned int CRC1,CRC2;
	
	ROMBootLoader = (const far rom char *)Boot_Loader_Start;
	CRC1 = CRC_CCITT(BootLoader,LoaderLength,(unsigned int)CRC_CCITT_SEED);
	CRC2 = CRC_CCITT(ROMBootLoader,LoaderLength,(unsigned int)CRC_CCITT_SEED);
	

	if(CRC1 != CRC2) {
		BootLoader_Update_ROM();

	}
}

void BootLoader_Update_ROM()
{
	char data[64];
	unsigned long ROMAddress = Boot_Loader_Start;
	char counter;
	unsigned int bytes_copied = 0;
	Interrupts_Disable_Global();

	while (bytes_copied < sizeof(BootLoader))
	{
		for(counter=0;counter < 64;counter+=1) {
			data[counter] = BootLoader[bytes_copied+counter];
		}

		// Erase block
		TBLPTR = ROMAddress;

		EECON1bits.EEPGD = 1;
		EECON1bits.CFGS = 0;
		EECON1bits.WREN = 1;
		EECON1bits.FREE = 1;
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1bits.WR=1;
	
		// Load the latch
		TBLPTR = ROMAddress;
		for(counter = 0;counter < 64;counter ++)
		{
			TABLAT = data[counter];
			_asm
				TBLWTPOSTINC
			_endasm
		}
		TBLPTR = ROMAddress;
		
		// Write the data
		EECON1bits.EEPGD = 1;
		EECON1bits.CFGS = 0;
		EECON1bits.WREN = 1;
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1bits.WR = 1;
		EECON1bits.WREN = 0;
	
		ROMAddress += 64;
		bytes_copied +=64;
	}	
	Interrupts_Enable_Global();

}
