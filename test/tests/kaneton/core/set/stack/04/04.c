/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/core/set/stack/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2040]
 * updated       julien quintard   [sun dec  5 16:26:45 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "04.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_stack_04(void)
{
  i_set			id;
  s_iterator		it;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;
  t_id			obj;
  int			i;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(stack,
		  SET_OPTION_ALLOCATE,
                  sizeof (t_id),
		  &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * push
   */

  obj = 80LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 98654LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 42LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 122LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 45LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 64LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 90LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 12346LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 67LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 90LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  /*
   * pop
   */

  for (i = 0; i < 10; i++)
    if (set_pop(id) != STATUS_OK)
      TEST_ERROR("[set_pop] error");

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

  /*
   * push
   */

  obj = 18LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 2LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 3LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 4LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 8LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  obj = 16LL;
  if (set_push(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_push] error\n");

  /*
   * pop
   */

  for (i = 0; i < 3; i++)
    if (set_pop(id) != STATUS_OK)
      TEST_ERROR("[set_pop] error");

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

  /*
   * release
   */

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(sdlkf34gi30wh);

  TEST_LEAVE();
}
