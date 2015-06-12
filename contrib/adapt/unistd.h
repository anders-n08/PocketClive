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

#ifndef _UNISTD_H_
#define _UNISTD_H_

/* --- defines, typedefs etc -------------------------------------------------- */

/* open flags */
#define O_RDONLY	1
#define O_WRONLY	2
#define O_RDWR		3
#define O_NONBLOCK	4
#define O_APPEND	5
#define O_CREAT		6
#define O_TRUNC		7
#define O_EXCL		8
#define O_NOCTTY	9
#define O_SHLOCK	10
#define O_EXLOCK	11

#define PROT_READ 0
#define MAP_SHARED 0


int open(char *path, int flags);
int close(int fd);
void *mmap(void *addr, int len, int prot, int flags, int fd, int offset);
int  munmap(void *addr, int len);

#endif

