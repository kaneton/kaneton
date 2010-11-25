/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/test/tests/kaneton/s../../../../01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu nov 18 05:59:27 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_ll_01(void)
{
  i_set			id;

  TEST_ENTER();

  if (set_reserve(ll, SET_OPTION_NONE, sizeof(t_id), &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error\n");

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error\n");

  if (set_reserve(ll, SET_OPTION_ALLOC | SET_OPTION_FREE,
                  sizeof(t_id), &id) == ERROR_OK)
    TEST_ERROR("[set_reserve] error: invalid arguments\n");

  if (set_reserve(ll, SET_OPTION_ORGANISE,
                  sizeof(t_id), &id) == ERROR_OK)
    TEST_ERROR("[set_reserve] error: invalid arguments\n");

  TEST_LEAVE();
}
