/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...test/tests/kaneton/core/set/pipe/09/09.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2090]
 * updated       julien quintard   [mon nov 29 19:17:04 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "09.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_pipe_09(void)
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

  if (set_reserve(pipe, SET_OPTION_ALLOCATE, sizeof (t_id), &id) != STATUS_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * push
   */

  for (i = 0; i < 20; ++i)
    {
      objs[i] = 4 * i;

      if (set_push(id, &objs[i]) != STATUS_OK)
        TEST_ERROR("[set_push] error");
    }

  /*
   * pick
   */

  if (set_size(id, &sz) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  TEST_PRINT("%qd elements: ", sz);
  st = 0;
  for (i = 0; i < sz; i++)
    {
      if (set_pick(id, (void**)&obj) != STATUS_OK)
        TEST_ERROR("[set_pick] error");

      if (!st++)
        TEST_PRINT("%qd", *obj);
      else
        TEST_PRINT(" %qd", *obj);

      if (set_pop(id) != STATUS_OK)
	TEST_ERROR("[set_pop] error");
    }
  TEST_PRINT("\n");

  /*
   * release
   */

  if (set_release(id) != STATUS_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(983r2uoskdfg34gh34h);

  TEST_LEAVE();
}
