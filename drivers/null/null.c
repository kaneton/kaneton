/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/drivers/null/null.c
 *
 * created       matthieu bucchianeri   [mon aug  6 22:05:34 2007]
 * updated       matthieu bucchianeri   [mon aug  6 22:13:20 2007]
 */

#include <libc.h>

int main()
{
  printf(" -- null: This is null device driver.\n");

  while (1)
    ;

  return (0);
}
