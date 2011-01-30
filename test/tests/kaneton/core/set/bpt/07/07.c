/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../test/tests/kaneton/core/set/bpt/07/07.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2070]
 * updated       julien quintard   [sun dec  5 16:24:00 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "07.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_bpt_07(void)
{
  i_set			id;
  s_iterator		it;
  t_id			obj;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;
  int			i;
  int			j;
  t_id			objs[10];

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(bpt,
		  SET_OPTION_SORT | SET_OPTION_ALLOCATE,
		  sizeof (t_id),
		  ___kaneton$pagesz,
		  &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * add
   */

  obj = 80LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 98654LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 42LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 122LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 45LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 64LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 90LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 12346LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 67LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 90LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  /*
   * remove
   */

  if (set_remove(id, 80LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

  if (set_remove(id, 98654LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

  if (set_remove(id, 42LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

  if (set_remove(id, 122LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

  if (set_remove(id, 45LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

  if (set_remove(id, 64LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

  if (set_remove(id, 90LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

  if (set_remove(id, 12346LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

  if (set_remove(id, 67LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

  if (set_remove(id, 90LL) != ERROR_OK)
    TEST_ERROR("[set_remove] error");

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
   * add
   */

  obj = 18LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 2LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 3LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 4LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 8LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  obj = 16LL;
  if (set_add(id, &obj) != ERROR_OK)
    TEST_PRINT("[set_add] error\n");

  /*
   * remove
   */

  i = 0;
  j = 0;
  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      t_id*	o;

      if (set_object(id, it, (void**)&o) != ERROR_OK)
        TEST_ERROR("[set_object] error");

      if (i++ % 2)
	objs[j++] = *o;
    }

  for (i = 0; i < j; i++)
    if (set_remove(id, objs[i]) != ERROR_OK)
      TEST_ERROR("[set_remove] error");

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

  TEST_SIGNATURE(f90wigi34htjy);

  TEST_LEAVE();
}
