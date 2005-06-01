/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/set/set_stack/10/10.c
 *
 * created       renaud voltz   [mon jan 30 20:57:06 2006]
 * updated       renaud voltz   [mon jan 30 20:57:06 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests with empty stacks
 */

void		check_set_stack_07(void)
{
  t_setid	id;
  t_iterator	it;
  t_state	state;
  t_id		obj;

  TEST_ENTER;

  if (set_reserve(stack, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error set_reserve()\n");

  if (set_pop(id) == ERROR_NONE)
    printf("error set_pop()\n");

  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      printf("error !\n");
    }

  obj = 42LL;
  if (set_push(id, &obj) != ERROR_NONE)
    printf("error set_push()\n");

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE;
}
