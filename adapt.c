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

/* This file implements all the Unix functionality defined in the .h files
   in the adapt directory, the purpose being to create a cradle for FUSE to
   be able to run on a PocketPC 
*/

/* Fake Unix includes */
#include "config.h"
#include "assert.h"
#include "unistd.h"
#include "timer.h"
#include "sys/stat.h"

/* Normal includes */
#include <stdio.h>
#include <string.h>

#include "pocket_clive_sound.h"

#include "contrib/fuse/types.h"
#include "contrib/fuse/settings.h"
#include "contrib/fuse/spectrum.h"
#include "contrib/fuse/joystick.h"

extern void plot_pixel( int x, int y, int color );

#define SP_WHITE 0xffff
#define SP_LBLACK 0
#define SP_LRED   ( 0x1f << 11 )
#define SP_LBLUE  ( 0x1f )
#define SP_LGREEN ( 0x3f << 5 )
#define SP_BLACK 0
#define SP_RED   ( 0x1b << 11 )
#define SP_BLUE  ( 0x1b )
#define SP_GREEN ( 0x37 << 5 )

int sp_color[ 16 ] = {
	SP_BLACK, 
	SP_BLUE, 
	SP_RED, 
	SP_BLUE + SP_RED,
	SP_GREEN, 
	SP_BLUE + SP_GREEN, 
	SP_RED + SP_GREEN, 
	SP_BLUE + SP_RED + SP_GREEN, 
	SP_LBLACK, 
	SP_LBLUE, 
	SP_LRED, 
	SP_LBLUE + SP_LRED,
	SP_LGREEN, 
	SP_LBLUE + SP_LGREEN, 
	SP_LRED + SP_LGREEN, 
	SP_LBLUE + SP_LRED + SP_LGREEN 
};

extern void vanta( void );

BYTE joystick = 0;

char *fuse_progname;
int fuse_sound_in_use;

typedef struct 
{
	FILE *fd_stream;
	int	 busy;
} fd_t;


int timer_count;


fd_t fd_array[] = { { NULL, 0 },
					{ NULL, 0 },
					{ NULL, 0 },
					{ NULL, 0 },
					{ NULL, 0 } };

int open( char *path, int flags )
{
	int i;

	/* This function modifies the given path to remove and / and
	   insert \ instead */
	while( strchr( path, '/' ) )
	{
		*(strchr( path, '/' )) = '\\'; 
	}

	/* Find free fd */
	for( i=0; i< ( sizeof( fd_array ) / sizeof( fd_t ) ); i++ )
	{
		if( !fd_array[ i ].busy )
		{
			if( flags && O_RDONLY )
			{
				fd_array[ i ].fd_stream = fopen( path, "rb" );
				if( fd_array[ i ].fd_stream != NULL )
				{
					fd_array[ i ].busy = 1;
					return i;
				}
			}
		}
	}

	/* Failed bitterly */
	return -1;
}

int close(int fd)
{
	if( fd_array[ fd ].busy )
	{
		fclose( fd_array[ fd ].fd_stream );
		fd_array[ fd ].busy = 0;
	}

	return 0;
}

void *mmap(void *addr, int len, int prot, int flags, int fd, int offset)
{
	char  *buffer = NULL, *ptr_buffer = NULL;
	int   file_size = 0;

	/* Always allocate 1M (we will change this later) */
	buffer = (char *)malloc( 1024 * 1024 );

	if( buffer )
	{
		ptr_buffer = buffer;

		while( fread( ptr_buffer, 1, 1, fd_array[ fd ].fd_stream ) )
		{
			ptr_buffer++;
			file_size++;

			/* This should never happen (how do we load a file bigger
			   than 128k into a Spectrum) */
			if( file_size >= ( 1024 * 1024 ) )
			{
				free( buffer );
				return (void *)-1;
			}
		}

		/* Reallocate buffer to correct size */
		buffer = realloc( buffer, file_size );

		return (void *)buffer;
	}

	return (void *)-1;
}

int munmap(void *addr, int len)
{
	free( addr );

	return 0;
}

int fstat(int fd, struct stat *sb)
{
	char buffer;
	int  file_size = 0;

	while( fread( &buffer, 1, 1, fd_array[ fd ].fd_stream ) )
	{
		file_size++;
	}

	fseek( fd_array[ fd ].fd_stream, 0, SEEK_SET );

	sb->st_size = file_size;

	return 0;
}

int perror( char *str )
{
	return 0;
}

void timer_sleep( void )
{
	vanta();
}

int ui_init(int *argc, char ***argv, int width, int height)
{
	return 0;
}

int ui_event(void)
{
	return 0;
}

int ui_end(void)
{
	return 0;
}

int uidisplay_init(int width, int height)
{
	return 0;
}

void uidisplay_putpixel(int x,int y,int colour)
{
	plot_pixel( x, y, sp_color[ colour ] );
}	

void uidisplay_line(int y)
{
	
}

void uidisplay_lines( int start, int end )
{

	
}

void uidisplay_set_border(int line, int pixel_from, int pixel_to, int colour)
{

}

int uidisplay_end(void)
{
	return 0;
}

int strncasecmp( const char *string1, const char *string2, size_t count )
{
	char temp_string1[ 100 ];
	char temp_string2[ 100 ];

	memset( temp_string1, '\0', 100 ); 
	memset( temp_string2, '\0', 100 ); 

	strncpy( temp_string1, string1, 99 );
	strncpy( temp_string2, string2, 99 );

	return( strncmp( _strlwr( temp_string1 ), _strlwr( temp_string2 ), count ) );
}

BYTE joystick_kempston_read(WORD port)
{
	if( !settings_current.joy_kempston )
		return spectrum_port_noread(port);

	return joystick;
}


void joystick_kempston_write(WORD port, BYTE b)
{
  /* nothing, presumably... */
}

void abort( void )
{

}










