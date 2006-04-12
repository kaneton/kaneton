/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/03/03.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:53:34 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_region_03(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_regid	reg;

  TEST_ENTER;
/*
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");
*/

  as = 0;

  MY_ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE, "error segment_reserve\n");

  MY_ASSERT(region_reserve(as,
			   seg,
			   0,
			   REGION_OPT_NONE,
			   0,
			   PAGESZ,
			   &reg) == ERROR_NONE, "error region_reserve\n");

  t_uint8* p = (t_uint8*)reg;
  for (; p < reg + PAGESZ; p++)
    *p = *p;

  MY_ASSERT(region_release(as, reg) == ERROR_NONE,
	    "faile to release region\n");

/*
  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");
*/
  TEST_LEAVE;
}
