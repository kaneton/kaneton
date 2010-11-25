/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...TABLE/test/tests/kaneton/set/bpt/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu nov 18 06:22:09 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_bpt_01(void)
{
  i_set			id;

  TEST_ENTER();

  if (set_reserve(bpt, SET_OPTION_SORT, sizeof(t_id), PAGESZ, &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error\n");

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error\n");

  if (set_reserve(bpt, SET_OPTION_ALLOC | SET_OPTION_FREE,
                  sizeof(t_id), PAGESZ, &id) == ERROR_OK)
    TEST_ERROR("[set_reserve] error: invalid arguments\n");

  if (set_reserve(bpt, SET_OPTION_ORGANISE,
                  sizeof(t_id), PAGESZ, &id) == ERROR_OK)
    TEST_ERROR("[set_reserve] error: invalid arguments\n");

  TEST_LEAVE();
}
