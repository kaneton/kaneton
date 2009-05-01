/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/test/tests/kaneton/id/clone/clone.c
 *
 * created       julien quintard   [wed apr 15 04:42:02 2009]
 * updated       julien quintard   [wed apr 15 07:54:18 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "clone.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void		test_id_clone(void)
{
  o_id		id;
  t_id		i;
  t_id		j;

  TESTS_ENTER();

  if (id_build(&id) != ERROR_NONE)
    printf("error id_build\n");
  else
    {
      if (id_reserve(&id, &i) != ERROR_NONE)
	printf("error id_reserve\n");

      if (!(i >= 0 && i <= (t_id)-1))
	printf("invalid id\n");

      if (id_clone(&id, i, &j) != ERROR_NONE)
	printf("error id_clone\n");

      if (!(j >= 0 && j <= (t_id)-1))
	printf("invalid id\n");

      if (id_release(&id, i) != ERROR_NONE)
	printf("error id_release\n");

      if (id_release(&id, j) != ERROR_NONE)
	printf("error id_release\n");

      if (id_destroy(&id) != ERROR_NONE)
	printf("error id_destroy\n");
    }

  TESTS_LEAVE();
}
