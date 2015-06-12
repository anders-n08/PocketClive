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

*/
#ifndef PC_SOUND_H
#define PC_SOUND_H

#include <winbase.h>
#include <mmsystem.h>

#define SOUND_OFF 0
#define SOUND_MUTE 1
#define SOUND_ON 2

extern int pc_sound_enabled;

MMRESULT pc_sound_init( void );
MMRESULT pc_sound_close( void );
void pc_sound_beeper( int on );
void pc_sound_frame( void );
char *pc_get_sound_buffer( void );
void pc_set_sound_volume( enum sound_mode_t sound_volume );
void write_silent_sound( void );

#endif