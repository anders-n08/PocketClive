/* config.h.in.  Generated automatically from configure.in by autoheader 2.13.  */
/* config.h.top: Preamble for config.h
   Copyright (c) 2000 Philip Kendall

   $Id: config.h.top,v 1.4 2001/08/13 21:38:52 pak21 Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

   Author contact information:

   E-mail: pak21-fuse@srcf.ucam.org
   Postal address: 15 Crescent Road, Wokingham, Berks, RG40 2DB, England

*/

/* Define to empty if the keyword does not work.  */
#undef const

/* Define if you have the ANSI C header files.  */
#undef STDC_HEADERS

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
#undef WORDS_BIGENDIAN

/* Define if the X Window System is missing or not being used.  */
#undef X_DISPLAY_MISSING

/* Defined if we've got enough memory to compile z80_ops.c */
#undef HAVE_ENOUGH_MEMORY

/* Defined if we've got glib */
#undef HAVE_LIB_GLIB

/* Defined if aalib UI in use */
#undef UI_AALIB

/* Defined if framebuffer UI in use */
#define UI_FB

/* Defined if GTK+ UI in use */
#undef UI_GTK

/* Defined if svgalib UI in use */
#undef UI_SVGA

/* Defined if Xlib UI in use */
#undef UI_X

/* The number of bytes in a char.  */
#define SIZEOF_CHAR 1

/* The number of bytes in a int.  */
#define SIZEOF_INT 2

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a short.  */
#define SIZEOF_SHORT 2

/* Define if you have the scandir function.  */
#undef HAVE_SCANDIR

/* Define if you have the setitimer function.  */
#undef HAVE_SETITIMER

/* Define if you have the <sys/soundcard.h> header file.  */
#define HAVE_SYS_SOUNDCARD_H

/* Define if you have the <sys/time.h> header file.  */
#undef HAVE_SYS_TIME_H

/* Define if you have the <unistd.h> header file.  */
#undef HAVE_UNISTD_H

/* Name of package */
#undef PACKAGE

/* Version number of package */
#undef VERSION


extern char ROMSDIR[];


/* for some reason eVC only has _snprintf, and not snprintf */
#define snprintf _snprintf


