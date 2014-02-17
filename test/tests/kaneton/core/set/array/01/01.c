/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...est/tests/kaneton/core/set/array/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 19:50:50 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_array_01(void)
{
  i_set			id;

  TEST_ENTER();

  if (set_reserve(array, SET_OPTION_NONE, 4, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  if (set_reserve(array, SET_OPTION_ALLOCATE | SET_OPTION_FREE, 4,
                  sizeof (t_id), &id) == STATUS_OK)
    TEST_ERROR("[set_reserve] error: invalid arguments");

  TEST_SIGNATURE(df093fiwg093h);

  TEST_LEAVE();
}
