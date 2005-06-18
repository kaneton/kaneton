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
 * last update   Sat Jun 18 15:46:31 2005   mycure
 */

#include <klibc/include/klibc.h>

void			bzero(void*		dst,
			      size_t		length)
{
  u_char		*data = dst;

  while (length-- > 0)
    *data++ = 0;
}
