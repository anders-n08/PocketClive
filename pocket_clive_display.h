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

#ifndef POCKET_CLIVE_DISPLAY_H
#define POCKET_CLIVE_DISPLAY_H

#include "winbase.h"
#include "pocket_clive_types.h"

extern unsigned int dirty_rec[ 24 ][ 32 ];

extern int color[ 16 ];

#undef  WHITE
#define WHITE 0xffff
#define LBLACK 0
#define LRED   ( 0x1f << 11 )
#define LBLUE  ( 0x1f )
#define LGREEN ( 0x3f << 5 )

#ifdef _X86_

#define BLACK 0
#define RED   ( 0x1f << 11 )
#define BLUE  ( 0x1f )
#define GREEN ( 0x3f << 5 )

#else

#define BLACK 0
#define RED   ( 0x1b << 11 )
#define BLUE  ( 0x1b )
#define GREEN ( 0x37 << 5 )

#endif

#define SCREEN_PORTRAIT        1
#define SCREEN_LANDSCAPE_LEFT  2
#define SCREEN_LANDSCAPE_RIGHT 3

typedef struct area_t 
{
	int            x1, y1, x2, y2;
	area_clicked_t area_clicked;
	int            clicktype;  /* Does this area react when pendown or clicked? */
	int						 x_offset, x_mult;
	int						 y_offset, y_mult;
	DWORD					 clocks_per_frame;
} area_t;

typedef struct area_t *area_ptr;
typedef area_ptr (*get_area_t)( int xpos, int ypos );

area_ptr get_area( int xpos, int ypos );
void set_area( area_ptr area, int nr_elements );

void show_spectrum_display( void );
void show_spectrum_display_landscape_right( void );
void all_is_dirty( void );
void flash_is_dirty( void );

#endif