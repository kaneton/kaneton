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
 * updated       julien quintard   [sat nov  6 14:21:25 2010]
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

  // XXX reverifier qu'on clean tout!

  if (id_build(&id) != ERROR_NONE)
    TEST_ERROR("[id_build] error\n");

  if (id_reserve(&id, &i) != ERROR_NONE)
    TEST_ERROR("[id_reserve] error\n");

  if (!(i >= 0 && i <= (t_id)-1))
    TEST_ERROR("invalid id\n");

  if (id_clone(&id, i, &j) != ERROR_NONE)
    TEST_ERROR("[id_clone] error\n");

  if (!(j >= 0 && j <= (t_id)-1))
    TEST_ERROR("invalid id\n");

  if (id_release(&id, i) != ERROR_NONE)
    TEST_ERROR("[id_release] error\n");

  if (id_release(&id, j) != ERROR_NONE)
    TEST_ERROR("[id_release] error\n");

  if (id_destroy(&id) != ERROR_NONE)
    TEST_ERROR("[id_destroy] error\n");

  TEST_LEAVE();
}
