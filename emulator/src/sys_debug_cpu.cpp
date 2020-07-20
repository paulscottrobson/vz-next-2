// ****************************************************************************
// ****************************************************************************
//
//		Name:		sys_debug_cpu.c
//		Purpose:	Debugger Code (System Dependent)
//		Created:	8th March 2020
//		Author:		Paul Robson (paul@robsons->org.uk)
//
// ****************************************************************************
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gfx.h"
#include "sys_processor.h"
#include "debugger.h"
#include "hardware.h"


#define DBGC_ADDRESS 	(0x0F0)														// Colour scheme.
#define DBGC_DATA 		(0x0FF)														// (Background is in main.c)
#define DBGC_HIGHLIGHT 	(0xFF0)

static int colours[8]; 

// ****************************************************************************
//							This renders the debug screen
// ****************************************************************************

static const char *labels[] = { "PC",
								"Clk","Brk",NULL };

void DBGXRender(int *address,int showDisplay) {

	int n = 0;
	char buffer[32];

	if (!showDisplay) {
		CPUSTATUS *s = CPUGetStatus();

		GFXSetCharacterSize(DBGXCHAR,DBGYCHAR);
		DBGVerticalLabel(24,0,labels,DBGC_ADDRESS,-1);									// Draw the labels for the register
		#define DN(v,w) GFXNumber(GRID(28,n++),v,16,w,GRIDSIZE,DBGC_DATA,-1)			// Helper macro
		DN(s->pc,4);
		DN(HWGetSystemClock(),4);
		DN(address[3],4);

		int a = address[1];																// Dump Memory.
		for (int row = 17;row < 30;row++) {
			GFXNumber(GRID(0,row),a,16,4,GRIDSIZE,DBGC_ADDRESS,-1);
			for (int col = 0;col < 8;col++) {
				BYTE8 b = CPUReadMemory(a);
				GFXNumber(GRID(5+col*3,row),b,16,2,GRIDSIZE,DBGC_DATA,-1);
				b = (b & 0x7F);b = (b < 0x20) ? '.':b;
				GFXCharacter(GRID(5+col+25,row),b,GRIDSIZE,DBGC_DATA,-1);
				a = (a + 1) & 0xFFFF;
			}		
		}

		int p = address[0];																// Dump program code. 
		int opc;

		for (int row = 0;row < 16;row++) {
			int isPC = (p == ((s->pc) & 0xFFFF));										// Tests.
			int isBrk = (p == address[3]);
			GFXNumber(GRID(0,row),p,16,4,
					  GRIDSIZE,isPC ? DBGC_HIGHLIGHT:DBGC_ADDRESS,	
					  isBrk ? 0xF00 : -1);
			int n = CPUReadMemory(p++);
			//GFXString(GRID(5,row),buffer,
			//		  GRIDSIZE,isPC ? DBGC_HIGHLIGHT:DBGC_DATA,	
			//		  isBrk ? 0xF00 : -1);
		}
	}

	#define CMAP(x,s) ((((x) & 1) ? 0xF:0x0) << (s))

	if (showDisplay) {
		for (int i = 0;i < 8;i++) {
			colours[i] = CMAP(i >> 2,0)+CMAP(i >> 1,4)+CMAP(i,8);
		}
		int scale = 3;
		SDL_Rect rc;rc.w = DWIDTH * scale;rc.h = DHEIGHT * scale;
		rc.x = WIN_WIDTH/2-rc.w/2;rc.y = WIN_HEIGHT/2-rc.h/2;
		#ifdef EMSCRIPTEN
		rc.x = rc.y = 0;
		#endif
		SDL_Rect rp;rp.w = rp.h = scale;
		GFXRectangle(&rc,0x000);
		//GFXRectangle(&rc,0x000);
	}
}	
