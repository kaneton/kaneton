/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/set/set_pipe/06/06.c
 *
 * created       renaud voltz   [mon jan 30 20:33:36 2006]
 * updated       renaud voltz   [mon jan 30 20:33:36 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests pick
 */

void		check_set_pipe_09(void)
{
  i_set	id;
  t_id		objs[64];
  t_id*		obj;
  t_setsz	i;
  int		f = 0;

  TEST_ENTER;

  if (set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 20; ++i)
    {
      objs[i] = 4 * i;
      if (set_push(id, &objs[i]) != ERROR_NONE)
        printf("error set_push()\n");
    }

  for (i = 0; i < 20; ++i)
    {
      if (set_pick(id, (void**)&obj) != ERROR_NONE)
	printf("error set_pick()\n");
      printf("%s%qd", f ? " " : "", *((i_set*)obj));
      set_pop(id);
      f = 1;
    }
  printf("\n");

  set_release(id);

  TEST_LEAVE;
}
