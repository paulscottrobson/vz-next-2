// ****************************************************************************
// ****************************************************************************
//
//		Name:		sys_processor.c
//		Purpose:	Processor Emulation.
//		Created:	8th March 2020
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// ****************************************************************************
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "sys_processor.h"
#include "sys_debug_system.h"
#include "hardware.h"

// ****************************************************************************
//							CPU / Memory
// ****************************************************************************

static BYTE8 ramMemory[0x10000];
static LONG32 cycles,frameCycles;
static LONG32 iCount;
static WORD16 instReg;
static WORD16 timer;
static BYTE8  turbo;

// ****************************************************************************
//							CPU Registers
// ****************************************************************************

WORD16 	pc;

// ****************************************************************************
//						Memory read and write macros.
// ****************************************************************************

#define FETCH() 			READ(pc++)
#define READ(addr) 			ramMemory[addr]
#define WRITE(addr,data)	ramMemory[addr] = data;

// ****************************************************************************
//								Get internal timer
// ****************************************************************************

WORD16 CPUGetEmulatedTimer(void) {
	return timer;
}

// ****************************************************************************
//								Reset the CPU
// ****************************************************************************

#ifdef INCLUDE_OS_SUPPORT
static void CPULoadChunk(FILE *f,BYTE8* memory,int count);
#endif

void CPUReset(void) {
	static BYTE8 ramInit = 0;
	iCount = cycles = 0;
	frameCycles = CYCLES_PER_FRAME;
}

// ****************************************************************************
//		Called on exit, does nothing on ESP32 but required for compilation
// ****************************************************************************

#ifdef INCLUDE_OS_SUPPORT
#include "gfx.h"
void CPUExit(void) {
	GFXExit();
}
#else
void CPUExit(void) {}
#endif

// ****************************************************************************
//							Execute a single instruction
// ****************************************************************************

BYTE8 CPUExecuteInstruction(void) {
	instReg = FETCH();																// Fetch opcode.
	switch(instReg) {																// Execute it.
	}
	cycles++;
	if (cycles < frameCycles) return 0;												// Not completed a frame.
	cycles = cycles - frameCycles;													// Adjust this frame rate.
	timer += 2;																		// Fix up 100Hz timer.
	iCount += frameCycles;
	HWSync(iCount);																	// Update any hardware
	return FRAME_RATE;																// Return frame rate.
}

// ****************************************************************************
//		 						Read/Write Memory
// ****************************************************************************

WORD16 CPUReadMemory(WORD16 address) {
	return READ(address);
}

void CPUWriteMemory(WORD16 address,WORD16 data) {
	WRITE(address,data);
}

#ifdef INCLUDE_OS_SUPPORT

// ****************************************************************************
//		Execute chunk of code, to either of two break points or frame-out,
//		return non-zero frame rate on frame, breakpoint 0
// ****************************************************************************

BYTE8 CPUExecute(WORD16 breakPoint1,WORD16 breakPoint2) {
	WORD16 next;
	do {
		BYTE8 r = CPUExecuteInstruction();											// Execute an instruction
		if (r != 0) return r; 														// Frame out.
		next = -1;
	} while (pc != breakPoint1 && pc != breakPoint2 && next != 0);				// Stop on breakpoint or $FF break
	return 0;
}

// ****************************************************************************
//			Return address of breakpoint for step-over, or 0 if N/A
// ****************************************************************************

WORD16 CPUGetStepOverBreakpoint(void) {
	return 0;																		// Do a normal single step
}

void CPUEndRun(void) {
	FILE *f = fopen("memory.dump","wb");
	fwrite(ramMemory,1,sizeof(ramMemory),f);
	fclose(f);
}

static void CPULoadChunk(FILE *f,BYTE8 *memory,int count) {
	while (count != 0 && !feof(f)) {
		BYTE8 b = fgetc(f);
		count--;
		*memory++ = b;
	}
}

void CPULoadBinary(char *fileName) {
	FILE *f = fopen(fileName,"rb");
	if (f != NULL) {
		int addr = 0;
		CPULoadChunk(f,ramMemory,sizeof(ramMemory));
		fclose(f);
	}
}

// ****************************************************************************
//						Retrieve a snapshot of the processor
// ****************************************************************************

static CPUSTATUS st;																	// Status area

CPUSTATUS *CPUGetStatus(void) {
	st.pc = pc;
	return &st;
}

#endif