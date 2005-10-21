/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bzero.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:50:14 2005   mycure
 * last update   Sun Jun 19 22:23:34 2005   mycure
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
