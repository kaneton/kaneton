/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ABLE/test/tests/kaneton/set/stack/09/09.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2090]
 * updated       julien quintard   [thu nov 18 06:58:48 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "09.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_set_stack_09(void)
{
  t_id			objs[64];
  i_set			id;
  t_setsz		sz;
  int			st;
  int			i;
  t_id*			obj;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(stack, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * push
   */

  for (i = 0; i < 20; ++i)
    {
      objs[i] = 4 * i;

      if (set_push(id, &objs[i]) != ERROR_NONE)
        TEST_ERROR("[set_push] error\n");
    }

  /*
   * pick
   */

  if (set_size(id, &sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  printf("%qd elements: ", sz);
  st = 0;
  for (i = 0; i < sz; i++)
    {
      if (set_pick(id, (void**)&obj) != ERROR_NONE)
        TEST_ERROR("[set_pick] error\n");

      if (!st++)
        printf("%qd", *obj);
      else
        printf(" %qd", *obj);

      if (set_pop(id) != ERROR_NONE)
	TEST_ERROR("[set_pop] error\n");
    }
  printf("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}
