/*
    (C) 2001 Anders Holmberg

    This file is part of PocketClive.

    PocketClive is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PocketClive is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PocketClive; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Author contact information : aagholmberg@yahoo.se

    PocketClive uses code and inspiration from the following sources 
    (see AUTHORS.TXT for more details) :

    FUSE - The Free Unix Spectrum Emulator - (C) 1999-2001 Philip Kendall
		
    EasyCE - PocketPC game programming library - (C) 2001 Jacco Bikker

    MESS - Multi-Emulator Super System - (C) 1998-2001 MESS Team

    Note : Source code files present in the sub directory PocketClive\EasyCE
           does not comply with the GPL license. These files are (C) Jacco Bikker
           with the following attachment : 

           "If you want to use EasyCE for commercial applications
            you should let me know. I will ask you a small fee,
            since I spend a lot of time developing this software.
            For freeware products, EasyCE can be used freely."
*/


#ifndef POCKET_EMULATOR_H 
#define POCKET_EMULATOR_H

#include "pocket_clive_types.h"
#include "contrib/easyce/EasyCE.h"

enum speed_t {
	speed_full = 0x01, 
	speed_1_skip = 0x02, 
	speed_2_skip = 0x03, 
	speed_3_skip = 0x04
};

enum screen_mode_t {
	screen_zoom, 
	screen_middle_align, 
	screen_left_align,
	screen_right_align
};

enum sound_mode_t {
	sound_auto, 
	sound_max, 
	sound_medium, 
	sound_minimum, 
	sound_off
};

enum joystick_t {
	joystick_kempston, 
	joystick_user_defined
};

struct settings_t 
{
	int machine_setting;
	enum speed_t speed_setting;
	enum screen_mode_t screen_mode_setting;
	enum sound_mode_t sound_mode_setting;
	enum joystick_t joystick_setting;
};

extern struct settings_t current_settings;

void setup_machine( int choice );
void show_error_dialog( char *error_text );
void show_dialog( char *title_text, char *text );
int tape_open(void);

extern PIXEL* getbuffer( void );
extern void print( char* a_String, int x1, int y1, PIXEL color );
extern void line( int x1, int y1, int x2, int y2, PIXEL color );
extern void plot( int x1, int y1, PIXEL color );
extern void box( int x1, int y1, int x2, int y2, PIXEL color );
extern void bar( int x1, int y1, int x2, int y2, PIXEL color );
extern bool tgazload( char* file, int w, int h, PIXEL* dest );
extern void clear( unsigned short color );
extern void centre( char *a_String, int y, PIXEL color );

#endif