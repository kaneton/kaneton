/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...TABLE/test/tests/kaneton/set/bpt/06/06.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2060]
 * updated       julien quintard   [wed nov 17 22:18:13 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "06.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_bpt_06(void)
{
  i_set			id;
  t_iterator		it;
  t_id			obj;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(bpt,
		  SET_OPTION_SORT | SET_OPTION_ALLOC,
		  sizeof(t_id),
		  PAGESZ,
		  &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * add
   */

  obj = 80LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  obj = 98654LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  obj = 42LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  obj = 122LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  obj = 45LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  obj = 64LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  obj = 90LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  obj = 12346LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  obj = 67LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  obj = 90LL;
  if (set_add(id, &obj) != ERROR_OK)
    printf("[set_add] error\n");

  /*
   * insert
   */

  obj = 4LL;
  if (set_insert(id, &obj) == ERROR_OK)
    TEST_ERROR("[set_insert] error: bpt-based sets do "
	       "not support this operation\n");

  /*
   * append
   */

  obj = 123456LL;
  if (set_append(id, &obj) == ERROR_OK)
    TEST_ERROR("[set_append] error: bpt-based sets do "
	       "not support this operation\n");

  /*
   * head & next
   */

  if (set_head(id, &it) != ERROR_OK)
    TEST_ERROR("[set_head] error\n");

  if (set_next(id, it, &it) != ERROR_OK)
    TEST_ERROR("[set_next] error\n");

  /*
   * after
   */

  obj = 456LL;
  if (set_after(id, it, &obj) == ERROR_OK)
    TEST_ERROR("[set_after] error: bpt-based sets do "
	       "not support this operation\n");

  /*
   * before
   */

  obj = 454LL;
  if (set_before(id, it, &obj) == ERROR_OK)
    TEST_ERROR("[set_before] error: bpt-based sets do "
	       "not support this operation\n");

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_OK)
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

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}
