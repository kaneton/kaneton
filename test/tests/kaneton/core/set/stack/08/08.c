/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ABLE/test/tests/kaneton/set/stack/08/08.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2080]
 * updated       julien quintard   [thu nov 18 06:54:34 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "08.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_stack_08(void)
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

  if (set_reserve(stack, SET_OPTION_ALLOC, sizeof(t_id), &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * push
   */

  for (i = 0; i < 80; i++)
    {
      obj = i;

      if (set_push(id, &obj) != ERROR_OK)
	TEST_ERROR("[set_push] error\n");
    }
  i = 0;

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  set_foreach(SET_OPTION_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_OK)
        TEST_ERROR("[set_object] error\n");

      if (*pdata != i++)
	printf("invalid order for the element %qu\n", *pdata);
    }
  printf("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}
