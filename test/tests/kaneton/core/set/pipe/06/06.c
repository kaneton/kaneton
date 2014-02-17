/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/core/set/pipe/06/06.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2060]
 * updated       julien quintard   [thu dec 16 12:19:39 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "06.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_pipe_06(void)
{
  t_id			objs[10];
  i_set			id;
  s_iterator		it;
  t_id			obj;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;
  int			i;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(pipe, SET_OPTION_ALLOCATE, sizeof (t_id), &id) != STATUS_OK)
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
   * flush
   */

  if (set_flush(id) != STATUS_OK)
    TEST_ERROR("[set_flush] error");

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

  /*
   * reserve
   */

  if (set_reserve(pipe, SET_OPTION_ALLOCATE, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * push
   */

  for (i = 0; i < 10; i++)
    {
      objs[i] = i * 10;

      if (set_push(id, &objs[i]) != STATUS_OK)
	TEST_ERROR("[set_push] error");
    }

  /*
   * flush
   */

  if (set_flush(id) != STATUS_OK)
    TEST_ERROR("[set_flush] error");

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

  /*
   * reserve
   */

  if (set_reserve(pipe, SET_OPTION_FREE, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  for (i = 0; i < 10; i++)
    {
      t_id*	o = malloc(sizeof (t_id));

      *o = i * 10;

      if (set_push(id, o) != STATUS_OK)
	TEST_ERROR("[set_push] error");
    }

  /*
   * flush
   */

  if (set_flush(id) != STATUS_OK)
    TEST_ERROR("error: set_flush()");

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

  TEST_SIGNATURE(9f0i23pek34h);

  TEST_LEAVE();
}
