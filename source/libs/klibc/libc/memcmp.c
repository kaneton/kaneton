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
 * last update   Sun Jun 19 22:24:54 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

int			memcmp(const void*			s1,
			       const void*			s2,
			       size_t				n)
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
