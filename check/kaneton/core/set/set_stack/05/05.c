/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/set/set_stack/04/04.c
 *
 * created       renaud voltz   [mon jan 30 21:11:15 2006]
 * updated       renaud voltz   [mon jan 30 21:11:15 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests the iterators option.
 */

void		check_set_stack_05(void)
{
  i_set	id;
  t_iterator	it;
  t_state	state;
  t_id*		pdata;
  int		f = 0;

  TEST_ENTER();

  if (set_reserve(stack, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  check_many_push(id, 10, 80LL, 98654LL, 42LL, 122LL, 45LL, 64LL, 90LL, 12346LL,
		 67LL, 90LL);

  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	printf("error set_object\n");
      printf("%s%qd", (f ? " " : ""), *pdata);
      f = 1;
    }
  printf("\n");

  f = 0;
  set_foreach(SET_OPT_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	printf("error set_object\n");
      printf("%s%qd", (f ? " " : ""), *pdata);
      f = 1;
    }
  printf("\n");

  set_release(id);

  TEST_LEAVE();
}
