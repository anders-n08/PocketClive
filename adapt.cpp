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

int open(const char *path, int flags)
{
	int i;

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
	fclose( fd_array[ fd ].fd_stream );

	return 0;
}

 void *mmap(void *addr, int len, int prot, int flags, int fd, int offset)
{
	return NULL;
}

int  munmap(void *addr, int len)
{
	return 0;
}

 int fstat(int fd, struct stat *sb)
{
	return 0;
}

 int snprintf(char *str, size_t size, const char *format, ...)
{
	return 0;
}

int perror( char *str )
{
	return 0;
}

 void timer_sleep( void )
{}














