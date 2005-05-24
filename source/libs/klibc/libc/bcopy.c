/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bcopy.c
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:50:03 2005   mycure
 * last update   Sat Apr 23 16:53:06 2005   mycure
 */

#include <klibc/include/klibc.h>

void			bcopy(const void	*src,
			      void		*dst,
			      size_t		n)
{
  memcpy(dst, src, n);
}

