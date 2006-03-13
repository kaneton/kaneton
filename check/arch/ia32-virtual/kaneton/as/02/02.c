/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/as/02/02.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [mon mar 13 19:37:56 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../../../../check/common/common.h"

void		check_as_02(void)
{
  t_tskid	task;
  t_asid	as;
  o_as*		o;
  t_segid	seg;
  t_regid	reg;

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

  if (segment_reserve(as, 1024 * PAGESZ, PERM_READ | PERM_WRITE, &seg) !=
      ERROR_NONE)
    {
      printf("error reserving segment\n");
      TEST_LEAVE;
    }

  if (region_reserve(as, seg, 0, REGION_OPT_NONE, 1024 * PAGESZ,
		     1024 * PAGESZ, &reg) != ERROR_NONE)
    {
      printf("error reserving region\n");
      TEST_LEAVE;
    }

  as_show(as);

  if (region_release(as, reg) != ERROR_NONE)
    printf("failed to release region");

  if (segment_release(seg) != ERROR_NONE)
    printf("failed to release segment");

  if (as_release(as) != ERROR_NONE)
    printf("failed to release as\n");

  if (task_release(task) != ERROR_NONE)
    printf("failed to release task\n");

  TEST_LEAVE;
}
