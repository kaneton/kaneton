/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibrary/libstring/strcpy.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:56:52 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			strcpy(char*				to,
			       const char*			from)
{
  u_int			i;

  for (i = 0; from[i]; i++)
    to[i] = from[i];
  to[i] = 0;

  return (to);
}
