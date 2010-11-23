/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ABLE/test/tests/kaneton/set/stack/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2040]
 * updated       julien quintard   [thu nov 18 07:01:20 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "04.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_set_stack_04(void)
{
  i_set			id;
  t_iterator		it;
  t_state		state;
  t_setsz		sz;
  void*			pdata;
  int			st;
  t_id			obj;
  int			i;

  TEST_ENTER();

  /*
   * reserve
   */

  if (set_reserve(stack,
		  SET_OPT_ALLOC,
                  sizeof(t_id),
		  &id) != ERROR_NONE)
    TEST_ERROR("[set_reserve] error\n");

  /*
   * push
   */

  obj = 80LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 98654LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 42LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 122LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 45LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 64LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 90LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 12346LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 67LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 90LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  /*
   * pop
   */

  for (i = 0; i < 10; i++)
    if (set_pop(id) != ERROR_NONE)
      TEST_ERROR("[set_pop] error\n");

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
   * push
   */

  obj = 18LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 2LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 3LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 4LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 8LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  obj = 16LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("[set_push] error\n");

  /*
   * pop
   */

  for (i = 0; i < 3; i++)
    if (set_pop(id) != ERROR_NONE)
      TEST_ERROR("[set_pop] error\n");

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
   * release
   */

  if (set_release(id) != ERROR_NONE)
    TEST_ERROR("[set_release] error\n");

  TEST_LEAVE();
}
