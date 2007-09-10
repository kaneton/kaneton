/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/set/set_pipe/07/07.c
 *
 * created       renaud voltz   [mon jan 30 20:43:15 2006]
 * updated       renaud voltz   [mon jan 30 20:43:15 2006]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests flush with all options modes
 */

void		check_set_pipe_06(void)
{
  i_set	id;
  t_id		objs[10];
  t_id		*obj;
  int		i;

  TEST_ENTER();

  if (set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  check_many_push(id, 10, 80LL, 98654LL, 42LL, 122LL, 45LL, 64LL, 90LL, 12346LL,
		 67LL, 90LL);

  if (set_flush(id) != ERROR_NONE)
    printf("error: set_flush()\n");

  check_display_set(id, NULL);

  set_release(id);

  if (set_reserve(pipe, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 10; i++)
    {
      objs[i] = i * 10;
      set_push(id, &objs[i]);
    }

  if (set_flush(id) != ERROR_NONE)
    printf("error: set_flush()\n");

  check_display_set(id, NULL);

  set_release(id);

  if (set_reserve(pipe, SET_OPT_FREE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 10; i++)
    {
      obj = malloc(sizeof(t_id));
      *obj = i * 10;
      set_push(id, obj);
    }

  if (set_flush(id) != ERROR_NONE)
    printf("error: set_flush()\n");

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE();
}
