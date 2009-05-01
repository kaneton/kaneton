/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibrary/libstring/memcpy.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:19 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>

/*
 * ---------- functions -------------------------------------------------------
 */

void*			memcpy(void*				dest,
			       const void*			src,
			       size_t				n)
{
  char*			d = dest;
  const char*		s = src;
  u_int			i;

  if ((n == 0) || (src == dest))
    return (NULL);

  /*
   * XXX bad performances
   */

  for (i = 0; i < n; i++)
    d[i] = s[i];

  return (d);
}
