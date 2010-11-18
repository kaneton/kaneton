/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ABLE/test/tests/kaneton/set/pipe/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu nov 18 05:26:59 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_set_pipe_01(void)
{
  i_set			id;

  TEST_ENTER();

  if (set_reserve(pipe, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  if (set_reserve(pipe, SET_OPT_ALLOC | SET_OPT_FREE,
                  sizeof(t_id), &id) == ERROR_NONE)
    TEST_ERROR("[set_reserve] error: invalid arguments\n");

  if (set_reserve(pipe, SET_OPT_ORGANISE,
                  sizeof(t_id), &id) == ERROR_NONE)
    TEST_ERROR("[set_reserve] error: invalid arguments\n");

  TEST_LEAVE();
}
