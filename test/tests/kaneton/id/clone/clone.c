/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ABLE/test/tests/kaneton/id/clone/clone.c
 *
 * created       julien quintard   [wed apr 15 04:42:02 2009]
 * updated       julien quintard   [mon oct 18 09:26:07 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "clone.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_id_clone(void)
{
  o_id			id;
  t_id			i;
  t_id			j;

  TEST_ENTER();

  if (id_build(&id) != ERROR_NONE)
    printf("[id_build] error\n");
  else
    {
      if (id_reserve(&id, &i) != ERROR_NONE)
	printf("[id_reserve] error\n");

      if (!(i >= 0 && i <= (t_id)-1))
	printf("invalid id\n");

      if (id_clone(&id, i, &j) != ERROR_NONE)
	printf("[id_clone] error\n");

      if (!(j >= 0 && j <= (t_id)-1))
	printf("invalid id\n");

      if (id_release(&id, i) != ERROR_NONE)
	printf("[id_release] error\n");

      if (id_release(&id, j) != ERROR_NONE)
	printf("[id_release] error\n");

      if (id_destroy(&id) != ERROR_NONE)
	printf("[id_destroy] error\n");
    }

  TEST_LEAVE();
}
