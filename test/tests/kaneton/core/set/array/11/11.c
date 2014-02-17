/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...est/tests/kaneton/core/set/array/11/11.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2110]
 * updated       julien quintard   [mon nov 29 19:08:20 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "11.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_array_11(void)
{
  i_set			id;
  s_iterator		it;
  t_id			obj;
  t_state		state;
  t_setsz		sz;
  t_id*			pdata;
  int			i;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(array, SET_OPTION_ALLOCATE, 10, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * add
   */

  for (i = 0; i < 130; i++)
    {
      obj = i;

      if (set_add(id, &obj) != STATUS_OK)
	TEST_ERROR("[set_add] error");
    }

  /*
   * display
   */

  if (set_size(id, &sz) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  TEST_PRINT("%qd elements: ", sz);
  set_foreach(SET_OPTION_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != STATUS_OK)
        TEST_ERROR("[set_object] error");

      if (*pdata != --i)
	TEST_PRINT("invalid order for the element %qu\n", *pdata);
    }
  TEST_PRINT("\n");

  /*
   * release
   */

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(09zsifeb903h3h3);

  TEST_LEAVE();
}
