// ****************************************************************************
// ****************************************************************************
//
//		Name:		hardware.h
//		Purpose:	Hardware Emulation Header
//		Created:	15th February 2020
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// ****************************************************************************
// ****************************************************************************

#ifndef _HARDWARE_H
#define _HARDWARE_H

#define DWIDTH 		(320)
#define DHEIGHT 	(240)

#ifdef WINDOWS
#define FILESEP 	'\\'
#else
#define FILESEP 	'/'
#endif

#ifdef ESP32
#endif

void HWSyncImplementation(LONG32 iCount);
void HWReset(void);
void HWSync(LONG32 iCount);
WORD16 HWGetSystemClock(void);

#endif
