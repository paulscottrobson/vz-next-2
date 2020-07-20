// ****************************************************************************
// ****************************************************************************
//
//		Name:		hardware.cpp
//		Purpose:	Hardware Emulation (Common)
//		Created:	8th March 2020
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// ****************************************************************************
// ****************************************************************************

#include "sys_processor.h"
#include "hardware.h"
#include "gfxkeys.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// ****************************************************************************
//							  Reset Hardware
// ****************************************************************************

void HWReset(void) {
}

// ****************************************************************************
//								  Sync CPU
// ****************************************************************************

void HWSync(LONG32 iCount) {
	HWSyncImplementation(iCount);
}

#if defined(WINDOWS) || defined(LINUX)
#include "hardware_emu.cpp"
#endif

#ifdef ESP32
#include "hardware_esp.cpp"
#endif

	