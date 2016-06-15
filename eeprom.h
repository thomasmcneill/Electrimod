/*! \file eeprom.h
    \brief Internal EEPROM routines

*/
#ifndef _EEPROM_H
#define _EEPROM_H

char EEPROM_Write(int address, char *data, int size);
char EEPROM_Read(unsigned int address, char *data, int size);
char EEPROM_Write_CRC(unsigned int address, void *data, int size);
char EEPROM_Read_CRC(unsigned int address, void *data, int size);


#endif
