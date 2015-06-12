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

/* This is a Unix adaptation layer for PocketPC - The functions and defintions
   in this file shall be moved to the "correct" locations if and only if I get
   the program actually running!

*/

#ifndef _STAT_H_
#define _STAT_H_

/* struct used by fstat() */
struct stat
{
	int st_size;
};

/* --- function definitions --------------------------------------------------- */
extern int fstat(int fd, struct stat *sb);

/* These really should be in stdio.h but since M$ has neglected to add this
   feature I just put it somewhere (it must be in one of the include files that
   FUSE acutally uses, otherwise I have to modify FUSE code)
*/
extern int perror( char *str );

#endif



