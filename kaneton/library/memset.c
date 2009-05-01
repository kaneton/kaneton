/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibrary/libstring/memset.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:15 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>

/*
 * ---------- functions -------------------------------------------------------
 */

void*			memset(void*				s,
			       int				c,
			       size_t				n)
{
  char*			d = s;
  u_int			i;

  for (i = 0; i < n; i++)
    d[i] = c;

  return (d);
}
