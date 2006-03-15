/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/07/07.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [wed mar 15 23:07:22 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../../../../check/common/common.h"

void		check_segment_07(void)
{
  t_tskid	task;
  t_asid	as;

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

  /* XXX */

  if (as_release(as) != ERROR_NONE)
    printf("failed to release as\n");

  if (task_release(task) != ERROR_NONE)
    printf("failed to release task\n");

  TEST_LEAVE;
}
