/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...E/test/tests/kaneton/core/set/ll/09/09.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2090]
 * updated       julien quintard   [sun dec  5 16:21:02 2010]
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
  s_iterator		it;
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

  if (set_reserve(ll, SET_OPTION_SORT, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * add
   */

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;

      if (set_add(id, &objs[i]) != STATUS_OK)
        TEST_ERROR("[set_add] error");
    }

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
   * locate
   */

  for (i = 0; i < 64; ++i)
    {
      if (i % 2)
        {
          if (set_locate(id, objs[i], &it) != STATUS_OK)
	    TEST_ERROR("[set_locate] error");
          else
            {
              if (set_object(id, it, (void**)&pdata) != STATUS_OK)
		TEST_ERROR("[set_object] error");

              if (*pdata != objs[i])
                TEST_ERROR("the retrieved object has an invalid identifier");
            }
        }
      else
        {
          if (set_get(id, objs[i], (void**)&pdata) != STATUS_OK)
	    TEST_ERROR("[set_get] error");

          if (*pdata != objs[i])
	    TEST_ERROR("the retrieved object has an invalid identifier");
        }
    }

  data = objs[5] - 1;

  if (set_locate(id, data, &it) == STATUS_OK)
    TEST_ERROR("[set_locate] error: found a non-existent item");

  /*
   * release
   */

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(klsdfmvoiqa2f43h);

  TEST_LEAVE();
}
