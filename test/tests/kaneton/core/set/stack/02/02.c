/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...est/tests/kaneton/core/set/stack/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [mon nov 29 19:17:52 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_stack_02(void)
{
  i_set			id;
  t_id			objs[64];
  t_setsz		i;
  s_iterator		it;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;

  TEST_ENTER();

  if (set_reserve(stack, SET_OPTION_NONE, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;

      if (set_push(id, &objs[i]) != STATUS_OK)
        TEST_ERROR("[set_push] error");
    }

  if (set_size(id, &sz) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  TEST_PRINT("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != STATUS_OK)
        TEST_ERROR("[set_object] error");

      if (!st++)
        TEST_PRINT("%qd", *((i_set*)pdata));
      else
	TEST_PRINT(" %qd", *((i_set*)pdata));
    }
  TEST_PRINT("\n");

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(xlkvog349gh34);

  TEST_LEAVE();
}
