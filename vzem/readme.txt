VZEM
VZ200/VZ300 Emulator for windows
Written by Guy Thomason
Email: intertek00@netspace.net.au
Build 20090820


Changes - 20th August 2009
--------------------------
- Added a dialog to display snapshot info: filetype, filename and start/end address
This will be displayed on the File -> SaveVZ action. The user now has the option of saving a basic or binary 
snapshot, and entering the filetype and filename (eg the filename that would be displayed in a CLOAD).
During a File -> LoadVZ action, the user has the option of displaying the snapshot details. This is configurable from 
Options -> Snapshots. The Fast Load option will just load the snapshot. 

- Improved split screen handling. Its not 100% yet but will run the standard demo. Needs tuning to adjust for HSYNC


 

Changes - 13th August 2009
--------------------------
- Added support for AY8910 soundchip. This is selectable from the options menu -> extended sound. 
To create a simple tone, enter the following commands from basic

OUT 129,8 		'volume register for channel a
OUT 128,15 		'volume data for register a
OUT 129,0 		'channel a fine register for channel a
OUT 128,49 		'fine note data for register a

James Diffendaffer has written an example program that demonstrates programming for the AY8910. 
It is included in this zip as:

forest.asm	assembler source	
forest.vz 	compiled snapshot



Changes - 27th October 2008
---------------------------
- Added toolbar. Functions from left to right are:
	Load snapshot
	Save VZ
	Paste text from clipboard
	Map printer
	Map drive 1
	Map drive 2
	Cassette play
	Cassette record
	Cassette stop
	Debugger
- Fixed printing, a listing from edasm now works
- Clicking close window in the debugger dialog exits the debugger
- Added right control and right shift keys
- Added equals key, maps the same as minus key
- Ability to load custom font (File - Load Font)


Changes - 21st October 2008
---------------------------
- Add Status bar for disk & tape 
- Save of vzem.cfg will save to correct directory  



Changes - 16th October 2008
---------------------------
- Add disk sound emulation 


Changes - 14th October 2008
---------------------------

- Add support for 2 disk drives
- Remove disk option now works


Changes - 9th October 2008
--------------------------

- Ability to load custom system and cartridge roms
- Option to write to system and cartridge memory
- Ability to save system and cartridge memory to binary file
- Option to enable/disable cassette audio when playing/recording
- Cassette controls smarter, eg if you are playing or recording the only option visible is to stop
- Load Bitmap function under Utils. This will load a 256x192 2 color windows bitmap to VZ memory at C000h (49152)
- Bank memory handling fixed 
- Enable Disk option changed to Enable Cartridge. This allows for other cartridge roms, eg the word processor



INTRODUCTION

VZEM is an emulator for the VZ200/VZ300 computers, also known as the Laser 200/ Laser 310 (Europe), Texet TX8000 (UK), and Salora Fellow (Finland).

The VZ200 was manufactured by Video Technology in Hong Kong in 1983. It was distributed in Australia by Dick Smith electronics, and was a popular first computer for many users due to its low cost. In 1983 the VZ200 sold for $199, the Commodore 64 by comparison sold for around $800.


HARDWARE EMULATION

Build 20080916 emulates the following hardware:

* Up to 34k RAM 
* Mode 0 and Mode 1 graphics
* Sound
* Joystick (via numeric keypad)
* Printer 
* Disk Drive
* Cassette Interface


EXTENDED GRAPHICS

In addition the “Super VZ Graphics” hardware modification is emulated. This provides support for all the graphics modes the video chip is capable of (Motorola 6847)

To active the extended graphics modes, type the following from the emulator:

GM0	OUT 32,0	64x64		Color			1024 bytes
GM1	OUT 32,4	128x64		Monochrome		1024 bytes
GM2	OUT 32,8	128x64		Color			2048 bytes
GM3	OUT 32,12	128x96		Monochrome		1536 bytes
GM4	OUT 32,16	128x96		Color			3072 bytes
GM5	OUT 32,20	128x192		Monochrome		3072 bytes
GM6	OUT 32,24	128x192		Color			6144 bytes
GM7	OUT 32,28	256x192		Monochrome		6144 bytes

Then activate the mode using the MODE(1) command or POKE 26624,8.

The VZ200 memory map can only access 2k video ram. For graphics modes that occupy more than 2k (eg GM7), bank switching is used. 

The byte written to port 32 is decoded via the following bits

xxxmmmbb

Where xxx is not used, mmm is the graphics mode, and bb is the bank. So to activate GM7 bank 0, the output byte would be 0011100 binary or 28 decimal. To activate GM7 bank 1, the output byte would be 0011101 binary or 29 decimal. 

For whatever bank is active, reads/writes to the 2k video memory at 7000h will affect the screen. For GM7, pixel rows 0-63 are active for bank 0, 64-127 for bank 1 and 128-191 for bank 2. 

For further information on extended vz graphics, read the following articles (available from VZAlive)

VZ Super Graphics by Joe Leon
Ultra Graphics Adapter by Matthew Sorrell.  

EXTENDED GRAPHICS UPDATED - 27/9/2008
=====================================

Support has been added for the German Graphics Mod. 
From the main menu, select options -> extended graphics -> german.
The German modification only supports GM7, the 256x192 monochrome mode. To enable the mode, set bit 4 of address 30779, eg POKE 30779,8
Then activate the hi-res screen by either the MODE(1) command or POKE 26624,8.

The bank switching is controlled by port 222, eg

OUT 222,0	selects bank 0
OUT 222,1	selects bank 1
OUT 222,2	selects bank 2
OUT 222,3	selects bank 3


EXTENDED SOUND - Added 4/10/2008

Support has been added for the "Compumuse" sound effects generator documented in the Electronics Australia August 1983 edition. 
This project is based on the Texas Instruments SN76489 sound chip, which has 3 independently programmable sound channels, plus a noise 
channel. Rather than use the printer interface (port 0), VZEM uses port
128 which is unused. 

To generate a 440hz tone from voice 0, enter the following commands from the emulator

OUT 128,142
OUT 128,8
OUT 128,144

The first two out commands define the sound channel (the first 1) and a 10 bit frequency value. The last out command sets the attenuation,
or volume, for channel 1. To turn off the sound, type

OUT 128,159

Which will set the maximum attenuation, turning the sound off. There are 16 volume levels, so the max volume for channel 1 is 144, and the
minimum volume for channel 1 is 159. The value sets the attenuation, which is really the volume in reverse. 



USAGE

VZEM requires the following files to exist in the same directory as vz.exe

* vzrom.v20		VZ200 rom image
* vzdos.rom		Dos rom image (if disk emulation is required)
* vzem.cfg		Configuration file

When vz.exe is first run it will look for vzem.cfg. If this is not found VZEM will create it with basic default configuration options.

From within the emulator, the hardware configuration can be changed from the “Options” menu.  To make this configuration permanent, select the “Save Config” option.


LOADING/SAVING files

Programs can be loaded or saved by:

* Snapshots (.VZ files)
* Cassette emulation using Windows Wav files
* Disk emulation using a disk image

To save a BASIC program to a snapshot image, select File - Save VZ
To save a BASIC program to a Wav file, follow the following steps
1. In the emulator, type CSAVE “filename” but do not press enter
2. Select File - Cassette - Record
3. Enter the name of the PC file you wish to create and click SAVE
4. Press enter from the emulator

You should hear the cassette tones as the file is being saved. 

5. Once the save is complete and the tones have stopped, select
       File - Cassette - Stop  
      
It is very important to “stop” the recording just as you would if you physically saved a program to tape. If you do not stop, the file length is indeterminate and the file will not be a valid wav file.

To save a program to disk, select File – Mount Disk. Select an existing disk image, or type the name of a new file. If using an existing disk image, the program can be saved with the SAVE “filename” command. When creating a new disk image, the disk must first be formatted with the INIT command. (See disk commands)


CREDITS

Thanks to the following people who directly supplied me with information, or that I have stolen ideas and code from:

Marat Fayzullin		Z80 Emulation library. His MSX emulator motived me to develop a VZ200 emulator   	 

Juergen Buchmueller	Disk emulation routines and other bits and pieces

Richard Wilson		Screen timing information for use in split screen modes


OTHER VZ200 EMULATORS

MESS				VZ200 driver by Juergen Buchmueller and Dirk Best
				http://www.mess.org/download.php

JEMU				VZ200 emulation in a browser! By Richard Wilson
				http://jemu.winape.net/

James VZ200			Lots of cool features like QuickType and configurable CPU Speed. By James Tamer
				http://www.geocities.com/emucompboy/

That’s a total of 4 emulators for the VZ! Quite a good turnout for an old 8 bitter


LINKS

Thanks to the work of VZ enthusiasts everywhere a google search of VZ200 returns pages of links. The major ones are:

http://vzalive.bangrocks.com/			downloads & discussion forum
http://www.vz200.org/ 				Archive of manuals and newsletters
http://archivz.all2ezy.com/			Archive of manuals and newsletters	

My own website, containing latest downloads of the emulator
http://intertek00.customer.netspace.net.au/vz200/

