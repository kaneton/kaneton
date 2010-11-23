/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/test/tests/kaneton/s../../../../10.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2100]
 * updated       julien quintard   [wed nov 17 22:56:06 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "10.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_ll_10(void)
{
  i_set			id;
  t_iterator		it;
  t_id			obj;
  t_state		state;
  t_setsz		sz;
  int			st;
  void*			pdata;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(ll, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * locate
   */

  if (set_locate(id, 42LL, &it) == ERROR_NONE)
    TEST_ERROR("[set_locate] error: found a non-existent item\n");

  /*
   * remove
   */

  if (set_remove(id, 42LL) == ERROR_NONE)
    TEST_ERROR("[set_remove] error: removed a non-existent item\n");

  /*
   * explore
   */

  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      TEST_ERROR("there should not be any item in the set\n");
    }

  /*
   * append
   */

  obj = 42LL;
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

  /*
   * release
   */

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}
