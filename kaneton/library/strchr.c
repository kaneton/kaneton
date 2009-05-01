/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibrary/libstring/strchr.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:57:00 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			strchr(const char*			p,
			       int				ch)
{
  u_int			i;

  for (i = 0; p[i]; i++)
    if (p[i] == ch)
      return ((char *) p + i);

  return (NULL);
}
