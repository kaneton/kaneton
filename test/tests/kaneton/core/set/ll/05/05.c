/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/test/tests/kaneton/s../../../../05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2050]
 * updated       julien quintard   [thu nov 18 06:20:50 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_set_ll_05(void)
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

  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * add
   */

  obj = 80LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

  obj = 98654LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

  obj = 42LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

  obj = 122LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

  obj = 45LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

  obj = 64LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

  obj = 90LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

  obj = 12346LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

  obj = 67LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

  obj = 90LL;
  if (set_add(id, &obj) != ERROR_NONE)
    TEST_ERROR("[set_add] error\n");

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

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPT_BACKWARD, id, &it, state)
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
