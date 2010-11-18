/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/test/tests/kaneton/set/ll/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [wed nov 17 22:55:04 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_set_ll_03(void)
{
  i_set			id;
  t_setsz		i;
  t_test_object		obj1;
  t_test_object*	obj2;
  t_iterator		it;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;

  TEST_ENTER();

  /*
   * OPT_ALLOC
   */

  if (set_reserve(ll,
		  SET_OPT_ALLOC,
		  sizeof(t_test_object),
		  &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  for (i = 0; i < 16; ++i)
    {
      obj1.id = (i * 234) % 6578;

      strcpy(obj1.str, "XXX");

      if (set_add(id, &obj1) != ERROR_NONE)
        TEST_ERROR("[set_add] error\n");
    }

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
        TEST_ERROR("[set_object] error\n");

      if (!st++)
        printf("%qd %s",
	       ((t_test_object*)pdata)->id, ((t_test_object*)pdata)->str);
      else
        printf(" %qd %s",
	       ((t_test_object*)pdata)->id, ((t_test_object*)pdata)->str);
    }
  printf("\n");

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  /*
   * OPT_FREE
   */

  if (set_reserve(ll,
		  SET_OPT_FREE,
		  sizeof(t_test_object),
		  &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  for (i = 0; i < 16; ++i)
    {
      obj2 = malloc(sizeof(t_test_object));

      obj2->id = (i * 234) % 6578;

      strcpy(obj2->str, "XXX");

      if (set_add(id, obj2) != ERROR_NONE)
        TEST_ERROR("[set_add] error\n");
    }

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
        TEST_ERROR("[set_object] error\n");

      if (!st++)
        printf("%qd %s",
	       ((t_test_object*)pdata)->id, ((t_test_object*)pdata)->str);
      else
        printf(" %qd %s",
	       ((t_test_object*)pdata)->id, ((t_test_object*)pdata)->str);
    }
  printf("\n");

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}
