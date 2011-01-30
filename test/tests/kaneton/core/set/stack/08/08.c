/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...est/tests/kaneton/core/set/stack/08/08.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2080]
 * updated       julien quintard   [mon nov 29 19:19:52 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "08.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_stack_08(void)
{
  i_set			id;
  s_iterator		it;
  t_id			obj;
  t_state		state;
  t_setsz		sz;
  t_id*			pdata;
  int			i;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(stack, SET_OPTION_ALLOCATE, sizeof (t_id), &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * push
   */

  for (i = 0; i < 80; i++)
    {
      obj = i;

      if (set_push(id, &obj) != ERROR_OK)
	TEST_ERROR("[set_push] error");
    }
  i = 0;

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  TEST_PRINT("%qd elements: ", sz);
  set_foreach(SET_OPTION_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_OK)
        TEST_ERROR("[set_object] error");

      if (*pdata != i++)
	TEST_PRINT("invalid order for the element %qu\n", *pdata);
    }
  TEST_PRINT("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(slvdlvk43gh43);

  TEST_LEAVE();
}
