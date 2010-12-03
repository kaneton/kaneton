/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...E/test/tests/kaneton/core/set/ll/06/06.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2060]
 * updated       julien quintard   [mon nov 29 19:22:15 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "06.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_ll_06(void)
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

  if (set_reserve(ll, SET_OPTION_ALLOC, sizeof(t_id), &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

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
  if (set_insert(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_insert] error");

  /*
   * append
   */

  obj = 123456LL;
  if (set_append(id, &obj) != ERROR_OK)
    TEST_ERROR("[set_append] error");

  /*
   * head & next
   */

  if (set_head(id, &it) == ERROR_FALSE)
    TEST_ERROR("[set_head] error");

  if (set_next(id, it, &it) == ERROR_FALSE)
    TEST_ERROR("[set_next] error");

  /*
   * after
   */

  obj = 456LL;
  if (set_after(id, it, &obj) != ERROR_OK)
    TEST_ERROR("[set_after] error");

  /*
   * before
   */

  obj = 454LL;
  if (set_before(id, it, &obj) != ERROR_OK)
    TEST_ERROR("[set_before] error");

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

  TEST_SIGNATURE(xcklvg34okgh);

  TEST_LEAVE();
}
