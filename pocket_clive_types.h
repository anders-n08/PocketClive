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


#ifndef POCKET_EMULATOR_TYPES_H
#define POCKET_EMULATOR_TYPES_H

#define CLICK_TYPE 1
#define PENDOWN_TYPE 2

typedef signed char     SBYTE;
typedef unsigned char   BYTE;
typedef signed short    SWORD;
typedef unsigned short  WORD;
//typedef signed long      SDWORD;
//typedef unsigned long    DWORD;
typedef unsigned int UINT;
typedef signed int SINT;

typedef BYTE (*read_memory_t)( WORD address );
typedef void (*write_memory_t)( WORD address, BYTE value );
typedef BYTE (*read_port_t)( WORD port );
typedef void (*write_port_t)( WORD port, BYTE value );

typedef void (*area_clicked_t)( int xpos, int ypos, int xchar, int ychar );


/* The number of bytes in a char.  */
#ifndef SIZEOF_CHAR
#define SIZEOF_CHAR 1
#endif

/* The number of bytes in a int.  */
#ifndef SIZEOF_INT
#define SIZEOF_INT 2
#endif

/* The number of bytes in a long.  */
#ifndef SIZEOF_LONG
#define SIZEOF_LONG 4
#endif

/* The number of bytes in a short.  */
#ifndef SIZEOF_SHORT
#define SIZEOF_SHORT 2
#endif

#endif	