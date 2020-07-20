// ****************************************************************************
// ****************************************************************************
//
//		Name:		sys_processor.h
//		Purpose:	Processor Emulation (header)
//		Created:	8th March 2020
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// ****************************************************************************
// ****************************************************************************

#ifndef _PROCESSOR_H
#define _PROCESSOR_H

typedef unsigned short WORD16;														// 8 and 16 bit types.
typedef unsigned char  BYTE8;
typedef unsigned int   LONG32;														// 32 bit type.

#define DEFAULT_BUS_VALUE (0xFF)													// What's on the bus if it's not memory.

void CPUReset(void);
BYTE8 CPUExecuteInstruction(void);
WORD16 CPUReadMemory(WORD16 address);
void CPUWriteMemory(WORD16 address,WORD16 data);
WORD16 CPUGetEmulatedTimer(void);

typedef struct __CPUSTATUS {
	int pc;
} CPUSTATUS;

#define CYCLE_RATE 		(1*1000*1000)												// Cycles per second (1Mhz)
#define FRAME_RATE		(50)														// Frames per second (50)
#define CYCLES_PER_FRAME (CYCLE_RATE / FRAME_RATE)									// Cycles per frame (20,000)

#ifdef INCLUDE_OS_SUPPORT								

CPUSTATUS *CPUGetStatus(void);
BYTE8 CPUExecute(WORD16 breakPoint1,WORD16 breakPoint2);
WORD16 CPUGetStepOverBreakpoint(void);
void CPUEndRun(void);
void CPULoadBinary(char *fileName);
void CPUExit(void);

#endif
#endif
