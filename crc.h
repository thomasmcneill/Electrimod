#ifndef __CRC_H
#define __CRC_H 

#define CRC_16_SEED 0xffff
#define CRC_CCITT_SEED 0xffff
#define CRC_iButton_SEED 0x0
#define CRC_XModem_SEED  0x0


extern unsigned int CRC_XModem(far rom char *Data,int length,unsigned int crc_seed);
extern unsigned int CRC_iButton(far rom char *Data,int length,unsigned int crc_seed);
extern unsigned int CRC_CCITT(far rom char *Data,int length,unsigned int crc_seed);
extern unsigned int CRC_16(far rom char *Data,int length,unsigned int crc_seed);
extern void CRC_CCITT_CHAR(char c,unsigned int *crc);
#endif
