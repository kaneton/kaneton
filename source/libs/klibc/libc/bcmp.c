/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bcmp.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:49:47 2005   mycure
 * last update   Sat Jun 18 15:46:13 2005   mycure
 */

#include <klibc/include/klibc.h>

int			bcmp(const void*	b1,
			     const void*	b2,
			     size_t		length)
{
  const char*		p1 = b1;
  const char*		p2 = b2;

  if (length == 0)
    return(0);

  do
    {
      if (*p1++ != *p2++)
	break;
    } while (--length);
  
  return (length);
}
