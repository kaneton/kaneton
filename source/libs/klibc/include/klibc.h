/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * klibc.h
 * 
 * path          /home/mycure/kaneton/src/libs
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 13:53:34 2005   mycure
 * last update   Fri May  6 00:09:55 2005   mycure
 */

#ifndef KLIBC_H
#define KLIBC_H			1

/*
 * general defines
 */

#define NULL		((void *) 0)

#ifndef FALSE
#define FALSE		(0 == 1)
#endif

#ifndef TRUE
#define TRUE		(1 == 1)
#endif

#define EOF		-1

/*
 * endian byte order definitions
 */

#define LITTLE_ENDIAN	1234		/* LSB first: i386, vax ... */
#define BIG_ENDIAN	4321		/* MSB first: 68000, ibm, net ... */
#define PDP_ENDIAN	3412		/* LSB first in word,
					   MSW first in long */

/*
 * includes
 */

#include <klibc/include/libsys/libsys.h>
#include <klibc/include/libc/libc.h>

extern int	errno;
extern int	suberrno;

#endif
