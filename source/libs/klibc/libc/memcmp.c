/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * memcmp.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:51:21 2005   mycure
 * last update   Sat Jun 18 15:48:18 2005   mycure
 */

#include <klibc/include/klibc.h>

int			memcmp(const void*	s1,
			       const void*	s2,
			       size_t n)
{
  if (n != 0)
    {
      const u_char*	p1 = s1;
      const u_char*	p2 = s2;

      do
        {
          if (*p1++ != *p2++)
            return (*--p1 - *--p2);
	} while (--n != 0);
    }

  return (0);
}
