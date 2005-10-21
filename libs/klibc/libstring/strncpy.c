/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strncpy.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:05:19 2005   mycure
 * last update   Sun Jun 19 22:28:58 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>

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
