/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/core/set/stack/07/07.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2070]
 * updated       julien quintard   [sun dec  5 16:27:22 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "07.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_stack_07(void)
{
  i_set			id;
  t_iterator		it;
  t_id			obj;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(stack, SET_OPTION_ALLOC, sizeof(t_id), &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * pop
   */

  if (set_pop(id) == ERROR_OK)
    TEST_ERROR("[set_pop] error: removed a non-existent item");

  /*
   * explore
   */

  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      TEST_ERROR("there should not be any item in the set");
    }

  /*
   * push
   */

  obj = 42LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  TEST_PRINT("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_OK)
        TEST_ERROR("[set_object] error");

      if (!st++)
        TEST_PRINT("%qd",
		   *((t_id*)pdata));
      else
        TEST_PRINT(" %qd",
		   *((t_id*)pdata));
    }
  TEST_PRINT("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(soifkwo3kg43h09);

  TEST_LEAVE();
}
