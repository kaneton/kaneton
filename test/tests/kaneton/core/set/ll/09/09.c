/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...E/test/tests/kaneton/core/set/ll/09/09.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2090]
 * updated       julien quintard   [mon nov 29 19:23:16 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "09.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_ll_09(void)
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

  if (set_reserve(ll, SET_OPTION_SORT, sizeof(t_id), &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * add
   */

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;

      if (set_add(id, &objs[i]) != ERROR_OK)
        TEST_ERROR("[set_add] error");
    }

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
   * locate
   */

  for (i = 0; i < 64; ++i)
    {
      if (i % 2)
        {
          if (set_locate(id, objs[i], &it) != ERROR_OK)
	    TEST_ERROR("[set_locate] error");
          else
            {
              if (set_object(id, it, (void**)&pdata) != ERROR_OK)
		TEST_ERROR("[set_object] error");

              if (*pdata != objs[i])
                TEST_ERROR("the retrieved object has an invalid identifier");
            }
        }
      else
        {
          if (set_get(id, objs[i], (void**)&pdata) != ERROR_OK)
	    TEST_ERROR("[set_get] error");

          if (*pdata != objs[i])
	    TEST_ERROR("the retrieved object has an invalid identifier");
        }
    }

  data = objs[5] - 1;

  if (set_locate(id, data, &it) == ERROR_OK)
    TEST_ERROR("[set_locate] error: found a non-existent item");

  /*
   * release
   */

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(klsdfmvoiqa2f43h);

  TEST_LEAVE();
}
