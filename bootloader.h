#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H 

#include <string.h>
#include <stdio.h>
#include <pwm.h>
#include <timers.h>
#include <p18F2525.h>
#include <math.h>
#include <adc.h>
#include <delays.h>
#include <reset.h>

#include "crc.h"

extern far rom char *ROMBootLoader;

extern void BootLoaderInit();
extern void BootLoader_Update_ROM();
extern void BootLoader_Start();


#endif
