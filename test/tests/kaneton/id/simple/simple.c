/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/test/tests/kaneton/id/simple/simple.c
 *
 * created       julien quintard   [wed apr 15 05:29:49 2009]
 * updated       julien quintard   [wed apr 15 07:55:15 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "simple.h"

/*
 * ---------- test ------------------------------------------------------------
 */

/*
 * basic tests
 */

void		test_id_simple(void)
{
  o_id		id;
  t_id		i;

  TESTS_ENTER();

  if (id_build(&id) != ERROR_NONE)
    printf("error id_build\n");
  else
    {
      if (id_reserve(&id, &i) != ERROR_NONE)
	printf("error id_reserve\n");

      if (!(i >= 0 && i <= (t_id)-1))
	printf("invalid id\n");

      if (id_release(&id, i) != ERROR_NONE)
	printf("error id_release\n");

      if (id_destroy(&id) != ERROR_NONE)
	printf("error id_destroy\n");
    }

  TESTS_LEAVE();
}
