/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bcopy.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:50:03 2005   mycure
 * last update   Sun Jun 19 22:23:27 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

void			bcopy(const void*			src,
			      void*				dst,
			      size_t				n)
{
  memcpy(dst, src, n);
}

