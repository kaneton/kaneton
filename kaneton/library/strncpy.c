/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/klibrary/libstring/strncpy.c
 *
 * created       julien quintard   [fri feb 11 02:56:44 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:56:30 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <library/library.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			strncpy(char*				to,
				char*				from,
				size_t				n)
{
  u_int			i;

  for (i = 0; from[i] && i < n; i++)
    to[i] = from[i];

  if (i < n)
    to[i] = 0;

  return (to);
}
