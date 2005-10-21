/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strcpy.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:53:59 2005   mycure
 * last update   Sun Jun 19 22:27:58 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

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
