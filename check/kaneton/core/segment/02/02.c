/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/02/02.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sat mar 25 16:29:39 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * segment_flush
 */

void		check_segment_02(void)
{
  o_as*		o;
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_uint32	i;
  t_setsz	sz;

  TEST_ENTER();
  ASSERT(task_reserve(TASK_CLASS_PROGRAM,

			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  ASSERT(as_get(as, &o) == ERROR_NONE, "error getting as\n");

  for (i = 0; i < 10; i++)
    {
      ASSERT(segment_reserve(as,
				2 * (i + 1) * PAGESZ,
				PERM_READ | PERM_WRITE,
				&seg) == ERROR_NONE,
		"error reserving segment\n");
    }

  ASSERT(segment_flush(as) == ERROR_NONE, "error flushing segments\n");

  ASSERT(set_size(o->segments, &sz) == ERROR_NONE, "error in get size\n");

  ASSERT(sz == 0, "wrong size\n");

  ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
