/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/test/tests/kaneton/set/ll/11/11.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2110]
 * updated       julien quintard   [wed nov 17 23:24:15 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "11.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_set_ll_11(void)
{
  i_set			id;
  t_iterator		it;
  t_id			obj;
  t_state		state;
  t_setsz		sz;
  t_id*			pdata;
  int			i;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * add
   */

  for (i = 0; i < 130; i++)
    {
      obj = i;

      if (set_add(id, &obj) != ERROR_NONE)
	TEST_ERROR("[set_add] error\n");
    }

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
        TEST_ERROR("[set_object] error\n");

      if (*pdata != --i)
	printf("invalid order for the element %qu\n", *pdata);
    }
  printf("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}
