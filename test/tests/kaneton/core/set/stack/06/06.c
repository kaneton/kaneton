/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ABLE/test/tests/kaneton/set/stack/06/06.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2060]
 * updated       julien quintard   [thu nov 18 06:59:50 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "06.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_set_stack_06(void)
{
  t_id			objs[10];
  i_set			id;
  t_iterator		it;
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

  if (set_reserve(stack, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * push
   */

  obj = 80LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  obj = 98654LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  obj = 42LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  obj = 122LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  obj = 45LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  obj = 64LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  obj = 90LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  obj = 12346LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  obj = 67LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  obj = 90LL;
  if (set_push(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_push] error\n");

  /*
   * flush
   */

  if (set_flush(id) != ERROR_NONE)
    TEST_ERROR("[set_flush] error\n");

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
        TEST_ERROR("[set_object] error\n");

      if (!st++)
        printf("%qd",
	       *((t_id*)pdata));
      else
        printf(" %qd",
	       *((t_id*)pdata));
    }
  printf("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  /*
   * reserve
   */

  if (set_reserve(stack, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * push
   */

  for (i = 0; i < 10; i++)
    {
      objs[i] = i * 10;

      if (set_push(id, &objs[i]) != ERROR_NONE)
	TEST_ERROR("[set_push] error\n");
    }

  /*
   * flush
   */

  if (set_flush(id) != ERROR_NONE)
    TEST_ERROR("[set_flush] error\n");

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
        TEST_ERROR("[set_object] error\n");

      if (!st++)
        printf("%qd",
	       *((t_id*)pdata));
      else
        printf(" %qd",
	       *((t_id*)pdata));
    }
  printf("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  /*
   * reserve
   */

  if (set_reserve(stack, SET_OPT_FREE, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  for (i = 0; i < 10; i++)
    {
      t_id*	o = malloc(sizeof(t_id));

      *o = i * 10;

      if (set_push(id, o) != ERROR_NONE)
	TEST_ERROR("[set_push] error\n");
    }

  /*
   * flush
   */

  if (set_flush(id) != ERROR_NONE)
    TEST_ERROR("error: set_flush()\n");

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
        TEST_ERROR("[set_object] error\n");

      if (!st++)
        printf("%qd",
	       *((t_id*)pdata));
      else
        printf(" %qd",
	       *((t_id*)pdata));
    }
  printf("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}
