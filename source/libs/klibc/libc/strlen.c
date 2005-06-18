/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strlen.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:54:47 2005   mycure
 * last update   Sat Jun 18 15:51:52 2005   mycure
 */

#include <klibc/include/klibc.h>

size_t			strlen(const char*	str)
{
  u_int			i;

  for (i = 0; str[i]; i++)
    ;

  return (i);
}
