/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibc/libstring/index.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:27 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			index(const char*			p,
			      int				ch)
{
  u_int			i;

  for (i = 0; p[i]; i++)
    if (p[i] == ch)
      return ((char *) p + i);

  return (NULL);
}
