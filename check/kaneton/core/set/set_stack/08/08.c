/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/set/set_stack/11/11.c
 *
 * created       renaud voltz   [mon jan 30 21:03:54 2006]
 * updated       renaud voltz   [mon jan 30 21:03:54 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests on a large stack
 */

void		check_set_stack_08(void)
{
  t_id		id;
  t_iterator	it;
  t_state	state;
  t_setsz	i;
  t_id*		pdata;

  TEST_ENTER();

  if (set_reserve(stack, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    printf("error set_reserve()\n");

  for (i = 0; i < 80; i++)
    {
      check_many_push(id, 1, i);
    }

  set_size(id, &i);
  printf("%qd\n", i);

  i = 0;
  set_foreach(SET_OPT_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	printf("error set_object()\n");
      else
	{
	  if (*pdata != i++)
	    printf("bad order %qd!\n", *pdata);
	}
    }

  set_release(id);

  TEST_LEAVE();
}
