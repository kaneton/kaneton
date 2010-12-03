/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../test/tests/kaneton/core/set/bpt/10/10.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2100]
 * updated       julien quintard   [mon nov 29 19:13:31 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "10.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_bpt_10(void)
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

  if (set_reserve(bpt, SET_OPTION_SORT, sizeof(t_id), PAGESZ, &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * locate
   */

  if (set_locate(id, 42LL, &it) == ERROR_OK)
    TEST_ERROR("[set_locate] error: found a non-existent item");

  /*
   * remove
   */

  if (set_remove(id, 42LL) == ERROR_OK)
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
  if (set_add(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_add] error");

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

  /*
   * release
   */

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(0g9iek4g3ih34hi9034);

  TEST_LEAVE();
}
