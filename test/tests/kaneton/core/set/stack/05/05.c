/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/core/set/stack/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2050]
 * updated       julien quintard   [sun dec  5 16:27:39 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_stack_05(void)
{
  i_set			id;
  s_iterator		it;
  t_state		state;
  t_id*			pdata;
  int			st;
  t_id			obj;
  t_setsz		sz;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(stack, SET_OPTION_ALLOCATE, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * push
   */

  obj = 80LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  obj = 98654LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  obj = 42LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  obj = 122LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  obj = 45LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  obj = 64LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  obj = 90LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  obj = 12346LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  obj = 67LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  obj = 90LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_push] error");

  /*
   * display
   */

  if (set_size(id, &sz) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  TEST_PRINT("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != STATUS_OK)
        TEST_ERROR("[set_object] error");

      if (!st++)
        TEST_PRINT("%qd",
		   *((t_id*)pdata));
      else
        TEST_PRINT(" %qd",
		   *((t_id*)pdata));
    }
  TEST_PRINT("\n");

  if (set_size(id, &sz) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  TEST_PRINT("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != STATUS_OK)
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

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(sklfopwkfg43gh0h);

  TEST_LEAVE();
}
