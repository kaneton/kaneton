/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...E/test/tests/kaneton/core/set/ll/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [sun dec  5 16:22:18 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_ll_03(void)
{
  i_set			id;
  t_setsz		i;
  t_test_object		obj1;
  t_test_object*	obj2;
  s_iterator		it;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;

  TEST_ENTER();

  /*
   * OPTION_ALLOC
   */

  if (set_reserve(ll,
		  SET_OPTION_ALLOCATE,
		  sizeof (t_test_object),
		  &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  for (i = 0; i < 16; ++i)
    {
      obj1.id = (i * 234) % 6578;

      strcpy(obj1.str, "XXX");

      if (set_add(id, &obj1) != STATUS_OK)
        TEST_ERROR("[set_add] error");
    }

  if (set_size(id, &sz) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  TEST_PRINT("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != STATUS_OK)
        TEST_ERROR("[set_object] error");

      if (!st++)
        TEST_PRINT("%qd %s",
		   ((t_test_object*)pdata)->id, ((t_test_object*)pdata)->str);
      else
        TEST_PRINT(" %qd %s",
		   ((t_test_object*)pdata)->id, ((t_test_object*)pdata)->str);
    }
  TEST_PRINT("\n");

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  /*
   * OPTION_FREE
   */

  if (set_reserve(ll,
		  SET_OPTION_FREE,
		  sizeof (t_test_object),
		  &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  for (i = 0; i < 16; ++i)
    {
      obj2 = malloc(sizeof (t_test_object));

      obj2->id = (i * 234) % 6578;

      strcpy(obj2->str, "XXX");

      if (set_add(id, obj2) != STATUS_OK)
        TEST_ERROR("[set_add] error");
    }

  if (set_size(id, &sz) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  TEST_PRINT("%qd elements: ", sz);
  st = 0;
  set_foreach(SET_OPTION_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != STATUS_OK)
        TEST_ERROR("[set_object] error");

      if (!st++)
        TEST_PRINT("%qd %s",
		   ((t_test_object*)pdata)->id, ((t_test_object*)pdata)->str);
      else
        TEST_PRINT(" %qd %s",
		   ((t_test_object*)pdata)->id, ((t_test_object*)pdata)->str);
    }
  TEST_PRINT("\n");

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(sdkvflksdmvo3p3h);

  TEST_LEAVE();
}
