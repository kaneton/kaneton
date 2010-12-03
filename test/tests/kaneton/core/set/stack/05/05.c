/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...est/tests/kaneton/core/set/stack/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2050]
 * updated       julien quintard   [mon nov 29 19:18:50 2010]
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
  t_iterator		it;
  t_state		state;
  t_id*			pdata;
  int			st;
  t_id			obj;
  t_setsz		sz;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(stack, SET_OPTION_ALLOC, sizeof(t_id), &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * push
   */

  obj = 80LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  obj = 98654LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  obj = 42LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  obj = 122LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  obj = 45LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  obj = 64LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  obj = 90LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  obj = 12346LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  obj = 67LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  obj = 90LL;
  if (set_push(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_push] error");

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_OK)
        TEST_ERROR("[set_object] error");

      if (!st++)
        printf("%qd",
	       *((t_id*)pdata));
      else
        printf(" %qd",
	       *((t_id*)pdata));
    }
  printf("\n");

  if (set_size(id, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_OK)
        TEST_ERROR("[set_object] error");

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

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(sklfopwkfg43gh0h);

  TEST_LEAVE();
}
