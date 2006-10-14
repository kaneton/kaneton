/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/set/set_pipe/01/01.c
 *
 * created       renaud voltz   [thu jan 26 00:42:17 2006]
 * updated       renaud voltz   [thu jan 26 00:42:17 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * create a pipe set with no option and release it. nothing more.
 */

void		check_set_pipe_01(void)
{
  i_set	id;

  TEST_ENTER();

  if (set_reserve(pipe, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  set_release(id);

  if (set_reserve(pipe, SET_OPT_ALLOC | SET_OPT_FREE,
		  sizeof(t_id), &id) == ERROR_NONE)
    printf("error: set_reserve() accepted bad arguments\n");

  if (set_reserve(pipe, SET_OPT_ORGANISE,
		  sizeof(t_id), &id) == ERROR_NONE)
    printf("error: set_reserve() accepted bad arguments\n");

  TEST_LEAVE();
}
