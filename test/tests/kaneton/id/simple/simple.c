/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...LE/test/tests/kaneton/id/simple/simple.c
 *
 * created       julien quintard   [wed apr 15 05:29:49 2009]
 * updated       julien quintard   [mon oct 18 09:40:29 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "simple.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_id_simple(void)
{
  o_id			id;
  t_id			i;

  TEST_ENTER();

  if (id_build(&id) != ERROR_NONE)
    printf("[id_build] error\n");
  else
    {
      if (id_reserve(&id, &i) != ERROR_NONE)
	printf("[id_reserve] error\n");

      if (!(i >= 0 && i <= (t_id)-1))
	printf("invalid id\n");

      if (id_release(&id, i) != ERROR_NONE)
	printf("[id_release] error\n");

      if (id_destroy(&id) != ERROR_NONE)
	printf("[id_destroy] error\n");
    }

  TEST_LEAVE();
}
