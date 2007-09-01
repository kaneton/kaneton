/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/libc/libstring/strrchr.c
 *
 * created       matthieu bucchianeri   [mon aug  6 21:54:06 2007]
 * updated       matthieu bucchianeri   [mon aug  6 21:56:48 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc/libc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

char*			strrchr(const char*			p,
				int				ch)
{
  char*			prev = NULL;

  for (; *p != 0; p++)
    if (*p == (char)ch)
      prev = (char*)p;

  return (prev);
}
