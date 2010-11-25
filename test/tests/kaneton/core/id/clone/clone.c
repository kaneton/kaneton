/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/core/id/clone/clone.c
 *
 * created       julien quintard   [wed apr 15 04:42:02 2009]
 * updated       julien quintard   [thu nov 18 16:15:27 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "clone.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_id_clone(void)
{
  o_id			id;
  t_id			i;
  t_id			j;

  TEST_ENTER();

  if (id_build(&id) != ERROR_OK)
    TEST_ERROR("[id_build] error\n");

  if (id_reserve(&id, &i) != ERROR_OK)
    TEST_ERROR("[id_reserve] error\n");

  if (!(i >= 0 && i <= (t_id)-1))
    TEST_ERROR("invalid id\n");

  if (id_clone(&id, i, &j) != ERROR_OK)
    TEST_ERROR("[id_clone] error\n");

  if (!(j >= 0 && j <= (t_id)-1))
    TEST_ERROR("invalid id\n");

  if (id_release(&id, i) != ERROR_OK)
    TEST_ERROR("[id_release] error\n");

  if (id_release(&id, j) != ERROR_OK)
    TEST_ERROR("[id_release] error\n");

  if (id_destroy(&id) != ERROR_OK)
    TEST_ERROR("[id_destroy] error\n");

  TEST_LEAVE();
}
