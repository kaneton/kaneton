/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * klibc.h
 * 
 * path          /home/mycure/kaneton/libs/klibc/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 13:53:34 2005   mycure
 * last update   Sat Jun 18 16:13:06 2005   mycure
 */

#ifndef KLIBC_H
#define KLIBC_H			1

/*
 * general defines
 */

#define NULL			((void *)0)

#ifndef FALSE
#define FALSE			(0 == 1)
#endif

#ifndef TRUE
#define TRUE			(1 == 1)
#endif

#define EOF			-1

/*
 * includes
 */

#include <klibc/include/libc/libc.h>
#include <klibc/include/libsys/libsys.h>

/*
 * errno
 */

extern int	errno;
extern int	suberrno;

#endif
