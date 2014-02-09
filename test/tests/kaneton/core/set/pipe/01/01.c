/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...test/tests/kaneton/core/set/pipe/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 19:14:35 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_pipe_01(void)
{
  i_set			id;

  TEST_ENTER();

  if (set_reserve(pipe, SET_OPTION_NONE, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  if (set_reserve(pipe, SET_OPTION_ALLOCATE | SET_OPTION_FREE,
                  sizeof (t_id), &id) == STATUS_OK)
    TEST_ERROR("[set_reserve] error: invalid arguments");

  if (set_reserve(pipe, SET_OPTION_ORGANISE,
                  sizeof (t_id), &id) == STATUS_OK)
    TEST_ERROR("[set_reserve] error: invalid arguments");

  TEST_SIGNATURE(f09ig390390iqawf);

  TEST_LEAVE();
}
