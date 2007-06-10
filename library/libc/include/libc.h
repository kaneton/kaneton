/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/library/libc/include/libc.h
 *
 * created       julien quintard   [sun jun 10 17:31:41 2007]
 * updated       julien quintard   [sun jun 10 17:32:22 2007]
 */

#ifndef LIBC_LIBC_H
#define LIBC_LIBC_H			1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define NULL			((void*)0)

#ifndef FALSE
#define FALSE			(0 == 1)
#endif

#ifndef TRUE
#define TRUE			(1 == 1)
#endif

#define EOF			-1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libdata/libdata.h>
#include <libstring/libstring.h>
#include <libsys/libsys.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/* XXX
extern int	errno;
extern int	suberrno;
*/

#endif
