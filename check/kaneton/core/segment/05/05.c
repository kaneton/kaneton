/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/05/05.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sat mar 25 16:30:24 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * injecting segments.
 */

void		check_segment_05(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  o_segment	new_seg;
  o_segment*	o;

  TEST_ENTER;
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  new_seg.address = 0x400000;
  new_seg.perms = PERM_READ | PERM_EXEC;
  new_seg.size = 2 * PAGESZ;
  new_seg.type = SEGMENT_TYPE_MEMORY;

  MY_ASSERT(segment_inject(as, &new_seg, &seg) == ERROR_NONE,
	    "error injecting segment\n");

  MY_ASSERT((t_paddr)seg == 0x400000, "Bad segment id\n");

  MY_ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting as\n");

  MY_ASSERT(o->segid == seg, "Bad segid field\n");
  MY_ASSERT(o->asid == as, "Bad asid field\n");
  MY_ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field\n");
  MY_ASSERT(o->address == (t_uint32)seg, "Bad address field\n");
  MY_ASSERT(o->size == 2 * PAGESZ, "Bad size field\n");
  MY_ASSERT(o->perms == (PERM_READ | PERM_EXEC), "Bad perms field\n");

  MY_ASSERT(segment_release(seg) == ERROR_NONE, "error releasing segment\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
