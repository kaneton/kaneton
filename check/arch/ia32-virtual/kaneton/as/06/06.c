/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/06/06.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [wed mar 15 22:28:57 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_06(void)
{
  t_tskid	task;
  t_asid	as;
  o_as*		o;

  TEST_ENTER;

  if (task_reserve(TASK_CLASS_PROGRAM, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    {
      printf("error creating task\n");
      TEST_LEAVE;
    }

  if (as_reserve(task, &as) != ERROR_NONE)
    {
      printf("error creating as\n");
      TEST_LEAVE;
    }

  if (as_get(as, &o) != ERROR_NONE)
    printf("unable to get as\n");

  if (as_release(as) != ERROR_NONE)
    printf("failed to release as\n");

  if (task_release(task) != ERROR_NONE)
    printf("failed to release task\n");

  TEST_LEAVE;
}
