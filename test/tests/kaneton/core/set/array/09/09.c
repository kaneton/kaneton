/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...BLE/test/tests/kaneton/set/array/09/09.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2090]
 * updated       julien quintard   [wed nov 17 22:57:02 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "09.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_array_09(void)
{
  t_id			objs[64];
  i_set			id;
  t_iterator		it;
  t_state		state;
  t_setsz		sz;
  t_id*			pdata;
  int			st;
  int			i;
  t_id			data;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(array, SET_OPT_SORT, 4, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * add
   */

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;

      if (set_add(id, &objs[i]) != ERROR_NONE)
        TEST_ERROR("[set_add] error\n");
    }

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
   * locate
   */

  for (i = 0; i < 64; ++i)
    {
      if (i % 2)
        {
          if (set_locate(id, objs[i], &it) != ERROR_NONE)
	    TEST_ERROR("[set_locate] error\n");
          else
            {
              if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
		TEST_ERROR("[set_object] error\n");

              if (*pdata != objs[i])
                TEST_ERROR("the retrieved object has an invalid identifier\n");
            }
        }
      else
        {
          if (set_get(id, objs[i], (void**)&pdata) != ERROR_NONE)
	    TEST_ERROR("[set_get] error\n");

          if (*pdata != objs[i])
	    TEST_ERROR("the retrieved object has an invalid identifier\n");
        }
    }

  data = objs[5] - 1;

  if (set_locate(id, data, &it) == ERROR_NONE)
    TEST_ERROR("[set_locate] error: found a non-existent item\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}
