/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/set/set_stack/02/02.c
 *
 * created       renaud voltz   [thu jan 26 00:50:43 2006]
 * updated       renaud voltz   [thu jan 26 00:50:43 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * create a stack then push some data.
 */

void		check_set_stack_02(void)
{
  t_setid	id;
  t_id		objs[64];
  t_setsz	i;

  TEST_ENTER;

  if (set_reserve(stack, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;
      if (set_push(id, &objs[i]) != ERROR_NONE)
	printf("error set_push()\n");
    }

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE;
}
