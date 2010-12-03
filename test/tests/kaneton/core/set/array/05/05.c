/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...est/tests/kaneton/core/set/array/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2050]
 * updated       julien quintard   [mon nov 29 19:05:50 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_array_05(void)
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

  if (set_reserve(array, SET_OPTION_ALLOC, 4, sizeof(t_id), &id) != ERROR_OK)
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

  TEST_SIGNATURE(9aizds90icvw239g);

  TEST_LEAVE();
}
