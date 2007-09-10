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

#include <libc/libc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests pop
 */

void		check_set_pipe_04(void)
{
  i_set	id;

  TEST_ENTER();

  if (set_reserve(pipe, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  check_many_push(id, 10, 80LL, 98654LL, 42LL, 122LL, 45LL, 64LL, 90LL, 12346LL,
		 67LL, 90LL);

  check_many_pop(id, 10);

  check_display_set(id, NULL);

  check_many_push(id, 6, 18LL, 2LL, 3LL, 4LL, 8LL, 16LL);
  check_many_pop(id, 3);

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE();
}
