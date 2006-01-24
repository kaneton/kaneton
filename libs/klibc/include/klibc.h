/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/include/klibc.h
 *
 * created       julien quintard   [fri feb 11 13:53:34 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:51:37 2006]
 */

#ifndef KLIBC_H
#define KLIBC_H			1

/*
 * ---------- defines ---------------------------------------------------------
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
