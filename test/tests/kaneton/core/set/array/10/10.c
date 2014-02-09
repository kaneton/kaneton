/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/core/set/array/10/10.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2100]
 * updated       julien quintard   [sun dec  5 16:20:13 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "10.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_array_10(void)
{
  i_set			id;
  s_iterator		it;
  t_id			obj;
  t_state		state;
  t_setsz		sz;
  int			st;
  void*			pdata;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(array, SET_OPTION_NONE, 4, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * locate
   */

  if (set_locate(id, 42LL, &it) == STATUS_OK)
    TEST_ERROR("[set_locate] error: found a non-existent item");

  /*
   * remove
   */

  if (set_remove(id, 42LL) == STATUS_OK)
    TEST_ERROR("[set_remove] error: removed a non-existent item");

  /*
   * explore
   */

  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      TEST_ERROR("there should not be any item in the set");
    }

  /*
   * append
   */

  obj = 42LL;
  if (set_add(id, &obj) != STATUS_OK)
    TEST_ERROR("[set_add] error");

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

  TEST_SIGNATURE(0z9sdifw39ig34g);

  TEST_LEAVE();
}
