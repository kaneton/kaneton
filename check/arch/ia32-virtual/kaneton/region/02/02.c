/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/02/02.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [thu apr  6 12:20:35 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_region_02(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_regid	reg;
  t_uint32	i;
  o_as*		o;
  t_setsz	sz;

  TEST_ENTER;

  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(as_get(as, &o) == ERROR_NONE, "error getting as\n");

  for (i = 0; i < 10; i++)
    {
      MY_ASSERT(segment_reserve(as,
				PAGESZ,
				PERM_READ | PERM_WRITE,
				&seg) == ERROR_NONE,
		"error reserving segment\n");

      MY_ASSERT(region_reserve(as,
			       seg,
			       0,
			       REGION_OPT_NONE,
			       0,
			       PAGESZ,
			       &reg) == ERROR_NONE,
		"error reserving region\n");
    }

  MY_ASSERT(region_flush(as) == ERROR_NONE, "error flushing regions\n");

  MY_ASSERT(set_size(o->regions, &sz) == ERROR_NONE, "error in get size\n");

  MY_ASSERT(sz == 0, "wrong size\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
