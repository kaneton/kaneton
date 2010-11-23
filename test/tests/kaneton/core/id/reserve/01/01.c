/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/core/id/reserve/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu nov 18 16:16:51 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_id_reserve_01(void)
{
  o_id			id;
  t_id			i;

  TEST_ENTER();

  if (id_build(&id) != ERROR_NONE)
    TEST_ERROR("[id_build] error\n");

  if (id_reserve(&id, &i) != ERROR_NONE)
    TEST_ERROR("[id_reserve] error\n");

  if (!(i >= 0 && i <= (t_id)-1))
    TEST_ERROR("invalid id\n");

  if (id_release(&id, i) != ERROR_NONE)
    TEST_ERROR("[id_release] error\n");

  if (id_destroy(&id) != ERROR_NONE)
    TEST_ERROR("[id_destroy] error\n");

  TEST_LEAVE();
}
