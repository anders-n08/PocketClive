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

#include "config.h"

#include "pocket_clive.h"
#include "pocket_clive_display.h"
#include "contrib\easyce\EasyCE.h"

extern int ypitch, xpitch;

#define GREY_20 0xce59

int color[ 16 ] = {
	BLACK, 
	BLUE, 
	RED, 
	BLUE + RED,
	GREEN, 
	BLUE + GREEN, 
	RED + GREEN, 
	BLUE + RED + GREEN, 
	LBLACK, 
	LBLUE, 
	LRED, 
	LBLUE + LRED,
	LGREEN, 
	LBLUE + LGREEN, 
	LRED + LGREEN, 
	LBLUE + LRED + LGREEN 
};

unsigned int dirty_rec[ 24 ][ 32 ];
static area_ptr crnt_area;
static int crnt_nr_elements;

void show_dialog( char *title_text, char *text );
void show_error_dialog( char *error_text );

/* --- show_error_dialog ---
 *
 * Show a "modal" error dialog box
 *
 * ---
 */
void show_error_dialog( char *error_text )
{
  show_dialog( "ERROR MESSAGE", error_text );
}

/* --- show_dialog ---
 *
 * Show a "modal" dialog box
 *
 * ---
 */
void show_dialog( char *title_text, char *text )
{
	unsigned short *p = (unsigned short *)getbuffer();
	unsigned short *buffer;
  unsigned int i;
	char buffer_text[ 32 ];

	buffer = (unsigned short *)malloc( 100 * 240 * sizeof( unsigned short ) );
	p += 240 * 40;
	
	memcpy( buffer, p, 100 * 240 * sizeof( unsigned short ) );

	bar( 30, 40, 210, 139, 0xffff );
	box( 30, 40, 210, 139, BLACK );
	centre( title_text, 50, BLACK );
	
	for( i=0; i<( strlen( text ) / 20) + 1; i++ )
	{
		memcpy( buffer_text, text + 20 * i, 30 );
		buffer_text[ 20 ] = '\0';
		centre( buffer_text, 65 + 12 * i, BLACK );
	}

	centre( "CLICK TO CONTINUE", 125, BLACK );
	
	/* Wait for user click */
	while ( clicked() ) 
		update( TRUE, 320 );
	while ( !clicked() ) 
		update( TRUE, 320 );

	p = (unsigned short *)getbuffer();
	p += 240*40; 
	memcpy( p, buffer, 100 * 240 * sizeof( unsigned short ) );
	free( buffer );

}

/* --- Screen Area handling routines
 *
 * Routines that handle the conversion between pixel 
 * position and screen area
 *
 */

void set_area( area_ptr area, int nr_elements )
{
	crnt_area        = area;
	crnt_nr_elements = nr_elements;
}

area_ptr get_area( int xpos, int ypos )
{
	int i;
	area_ptr area = crnt_area;

	for( i=0; i<crnt_nr_elements; i++ )
	{
		if( ( xpos >= area->x1 ) && 
			  ( xpos <= area->x2 ) && 
			  ( ypos >= area->y1 ) && 
			  ( ypos <= area->y2 ) )
			return area; 
		area++;
	}

	return NULL;
}

#if 0

void all_is_dirty( void )
{
	WORD col, row;

	for( row=0; row<24; row++ )
		for( col=0; col<32; col++ )
			dirty_rec[ row ][ col ] = 1;
}

void flash_is_dirty( void )
{
	WORD col, row;
	BYTE attribute_byte;

	for( row=0; row<24; row++ )
	{
		for( col=0; col<32; col++ )
		{
			attribute_byte = ram[ machine.screen_page ][ ( col | ( row << 5 ) ) | 0x1800 ];
			
			if( attribute_byte >> 8 )
				dirty_rec[ row ][ col ];
		}
	}
}

/* --- show_spectrum_display ---
 *
 * Write the Spectrum screen to the EasyCE screen buffer
 *
 * ---
 */
void show_spectrum_display( void )
{
	WORD pixel, pixel_row, pixel_byte, row, col;
	WORD f_color, b_color, temp_color;
	WORD base_pixel_address;
	WORD left_xpos = 0;
	WORD nr_pixels = 8;
	BYTE attribute_byte;

  for( row=0; row<24; row++ )
	{
		left_xpos = 0;
		nr_pixels = 8;

		for( col=0; col<32; col++ )
		{
			if( dirty_rec[ row ][ col ] )
			{
				base_pixel_address = ( col & 0x001f ) | ( ( row & 0x0007 ) << 5 ) | ( ( row & 0x0018 ) << 8 );
				attribute_byte     = ram[ machine.screen_page ][ ( col | ( row << 5 ) ) | 0x1800 ];

				f_color = color[ ( attribute_byte & 0x07 ) + ( ( attribute_byte & 0x40 ) >> 3 ) ]; 
				b_color = color[ ( ( attribute_byte >> 3 ) & 0x07 ) + ( ( attribute_byte & 0x40 ) >> 3 ) ]; 

				if( ( attribute_byte >> 7 ) && ( flash ) )
				{
					temp_color = f_color;
					f_color    = b_color;
					b_color		 = temp_color;
				}

				for( pixel_row=0; pixel_row<8; pixel_row++ )
				{
					pixel_byte = ram[ machine.screen_page ][ ( base_pixel_address | ( pixel_row << 8 ) ) ];				
					
					for( pixel=0; pixel<nr_pixels; pixel++ )
					{
						if( pixel_byte & ( 0x01 << ( 7 - pixel ) ) )
							plot( left_xpos + pixel, row * 8 + pixel_row, f_color );
						else
							plot( left_xpos + pixel, row * 8 + pixel_row, b_color );
					}
				}

				dirty_rec[ row ][ col ] = 0;
			}

			if( col & 0x0001 )
			{
				left_xpos += 7;
				nr_pixels = 8;
			}
			else
			{
				left_xpos += 8;
				nr_pixels = 7;
			}		
		}
	}
}

/* --- show_spectrum_display_landscape_right ---
 *
 * Write the Spectrum screen to the EasyCE screen buffer
 *
 * 16 pixels from top (i.e. 16 pixels border "left" and "right")
 * 24 pixels from left (i.e. 24 pixels border "over" and "under")
 * 
 * ---
 */
void show_spectrum_display_landscape_right( void )
{
	WORD pixel, pixel_row, pixel_byte, row, col;
	WORD f_color, b_color, temp_color;
	WORD base_pixel_address;
	WORD left_xpos = 0;
	WORD nr_pixels = 8;
	BYTE attribute_byte;

  for( row=0; row<24; row++ )
	{
		left_xpos = 0;
		nr_pixels = 8;

		for( col=0; col<32; col++ )
		{
			if( dirty_rec[ row ][ col ] )
			{
				base_pixel_address = ( col & 0x001f ) | ( ( row & 0x0007 ) << 5 ) | ( ( row & 0x0018 ) << 8 );
				attribute_byte     = ram[ machine.screen_page ][ ( col | ( row << 5 ) ) | 0x1800 ];

				f_color = color[ ( attribute_byte & 0x07 ) + ( ( attribute_byte & 0x40 ) >> 3 ) ]; 
				b_color = color[ ( ( attribute_byte >> 3 ) & 0x07 ) + ( ( attribute_byte & 0x40 ) >> 3 ) ]; 

				if( ( attribute_byte >> 7 ) && ( flash ) )
				{
					temp_color = f_color;
					f_color    = b_color;
					b_color		 = temp_color;
				}

				for( pixel_row=0; pixel_row<8; pixel_row++ )
				{
					pixel_byte = ram[ machine.screen_page ][ ( base_pixel_address | ( pixel_row << 8 ) ) ];				
					
					for( pixel=0; pixel<nr_pixels; pixel++ )
					{
						if( pixel_byte & ( 0x01 << ( 7 - pixel ) ) )
							plot( 215 - row * 8 - pixel_row, 16 + left_xpos + pixel, f_color );
						else
							plot( 215 - row * 8 - pixel_row, 16 + left_xpos + pixel, b_color );
					}
				}

				dirty_rec[ row ][ col ] = 0;
			}

			if( col & 0x0001 )
			{
				left_xpos += 7;
				nr_pixels = 8;
			}
			else
			{
				left_xpos += 8;
				nr_pixels = 7;
			}		
		}
	}
}

#endif

extern "C" void plot_pixel( int x, int y, int color )
{
	if( current_settings.screen_mode_setting == screen_zoom )
	{
		y -= 24;
		x -= 32;

		if( ( x & 0x0f ) != 0x0f )
		{
			plot( x - ( x >> 4 ), y, color );
		}
	}
	else if( current_settings.screen_mode_setting == screen_middle_align )
	{
		y -= 24;
		x -= 40;

		if( x >= 0 && x < 240 )
		{
			plot( x, y, color );	
		}
	}
	else if( current_settings.screen_mode_setting == screen_left_align )
	{
		y -= 24;
		x -= 32;

		if( x >= 0 && x < 240 )
		{
			plot( x, y, color );	
		}
	}
	else if( current_settings.screen_mode_setting == screen_right_align )
	{
		y -= 24;
		x -= 48;

		if( x >= 0 && x < 240 )
		{
			plot( x, y, color );	
		}
	}
}