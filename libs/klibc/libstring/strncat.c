/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/libstring/strncat.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:56:39 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			strncat(char*				dst,
				char*				src,
				size_t				n)
{
  u_int			i;
  u_int			j;

  for (i = 0; dst[i]; i++)
    ;

  for (j = 0; src[j] && j < n; j++, i++)
    dst[i] = src[j];
  dst[i] = 0;

  return (dst);
}
