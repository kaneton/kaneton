/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/test/tests/kaneton/id/XXX/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [mon nov  8 10:01:15 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_id_reserve_02(void)
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
