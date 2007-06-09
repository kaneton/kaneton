/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/libstring/bcopy.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:51 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

void			bcopy(const void*			src,
			      void*				dst,
			      size_t				n)
{
  memcpy(dst, src, n);
}

