/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../test/tests/kaneton/core/set/bpt/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2040]
 * updated       julien quintard   [sun dec  5 16:22:46 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "04.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_bpt_04(void)
{
  i_set			id;
  s_iterator		it;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;
  t_id			obj;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(bpt,
		  SET_OPTION_SORT | SET_OPTION_ALLOCATE,
                  sizeof (t_id),
		  ___kaneton$pagesz,
		  &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * add
   */

  obj = 80LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 98654LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 42LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 122LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

  /*
   * remove
   */

  if (set_remove(id, 42LL) != STATUS_OK)
    TEST_PRINT("error in set_remove()\n");

  /*
   * add
   */

  obj = 45LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 64LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

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
   * add
   */

  obj = 987LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 368LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 2LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 123LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

  /*
   * remove
   */

  if (set_remove(id, 368LL) != STATUS_OK)
    TEST_PRINT("error in set_remove()\n");

  if (set_remove(id, 80LL) != STATUS_OK)
    TEST_PRINT("error in set_remove()\n");

  /*
   * add
   */

  obj = 123LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_PRINT("[set_add] error\n");

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

  TEST_SIGNATURE(0f9iw3gih30h934h);

  TEST_LEAVE();
}
