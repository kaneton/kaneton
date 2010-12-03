/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../test/tests/kaneton/core/set/bpt/08/08.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2080]
 * updated       julien quintard   [mon nov 29 19:12:53 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "08.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_bpt_08(void)
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

  if (set_reserve(bpt,
		  SET_OPTION_SORT | SET_OPTION_ALLOC,
		  sizeof(t_id),
		  PAGESZ,
		  &id) != ERROR_OK)
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
   * flush
   */

  if (set_flush(id) != ERROR_OK)
    TEST_ERROR("[set_flush] error");

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

  /*
   * reserve
   */

  if (set_reserve(bpt,
		  SET_OPTION_SORT | SET_OPTION_ALLOC,
		  sizeof(t_id),
		  PAGESZ,
		  &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * add
   */

  for (i = 0; i < 10; i++)
    {
      objs[i] = i * 10;

      if (set_add(id, &objs[i]) != ERROR_OK)
	TEST_ERROR("[set_add] error");
    }

  /*
   * flush
   */

  if (set_flush(id) != ERROR_OK)
    TEST_ERROR("[set_flush] error");

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

  /*
   * reserve
   */

  if (set_reserve(bpt,
		  SET_OPTION_SORT | SET_OPTION_FREE,
		  sizeof(t_id),
		  PAGESZ,
		  &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  for (i = 0; i < 10; i++)
    {
      t_id*	o = malloc(sizeof(t_id));

      *o = i * 10;

      if (set_add(id, o) != ERROR_OK)
	TEST_ERROR("[set_add] error");
    }

  /*
   * flush
   */

  if (set_flush(id) != ERROR_OK)
    printf("error: set_flush()\n");

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

  TEST_SIGNATURE(90fgi43gserg5h);

  TEST_LEAVE();
}
