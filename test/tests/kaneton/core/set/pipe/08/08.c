/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...test/tests/kaneton/core/set/pipe/08/08.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2080]
 * updated       julien quintard   [mon nov 29 19:16:46 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "08.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_pipe_08(void)
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

  if (set_reserve(pipe, SET_OPTION_ALLOC, sizeof(t_id), &id) != ERROR_OK)
    TEST_ERROR("[set_reserve] error");

  /*
   * push
   */

  for (i = 0; i < 80; i++)
    {
      obj = i;

      if (set_push(id, &obj) != ERROR_OK)
	TEST_ERROR("[set_push] error");
    }
  i = 0;

  /*
   * display
   */

  if (set_size(id, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error");

  printf("%qd elements: ", sz);
  set_foreach(SET_OPTION_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_OK)
        TEST_ERROR("[set_object] error");

      if (*pdata != i++)
	printf("invalid order for the element %qu\n", *pdata);
    }
  printf("\n");

  /*
   * release
   */

  if (set_release(id) != ERROR_OK)
    TEST_ERROR("[set_release] error");

  TEST_SIGNATURE(fi2oeksokgh4);

  TEST_LEAVE();
}
