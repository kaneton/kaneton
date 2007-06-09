/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/libstring/bzero.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:48 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

void			bzero(void*				dst,
			      size_t				length)
{
  u_char		*data = dst;

  while (length-- > 0)
    *data++ = 0;
}
