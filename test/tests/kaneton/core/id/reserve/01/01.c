/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...st/tests/kaneton/core/id/reserve/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:44:49 2010]
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

  if (id_build(&id) != STATUS_OK)
    TEST_ERROR("[id_build] error");

  if (id_reserve(&id, &i) != STATUS_OK)
    TEST_ERROR("[id_reserve] error");

  if (!(i >= 0 && i <= (t_id)-1))
    TEST_ERROR("invalid id");

  if (id_release(&id, i) != STATUS_OK)
    TEST_ERROR("[id_release] error");

  if (id_destroy(&id) != STATUS_OK)
    TEST_ERROR("[id_destroy] error");

  TEST_SIGNATURE(9i43sdkfwaoir23wt);

  TEST_LEAVE();
}
