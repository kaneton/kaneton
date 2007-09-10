/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/split_01/split_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:50:26 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * split.
 */

void		check_segment_split_01(void)
{
  i_task	task;
  i_as	as;
  i_segment	seg;
  i_segment	seg2;
  o_segment*	o;

  TEST_ENTER();
  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  ASSERT(segment_reserve(as,
			    3 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  ASSERT(segment_split(seg, 2 * PAGESZ, &seg, &seg2) == ERROR_NONE,
	    "error splitting\n");

  ASSERT(seg2 == seg + 2 * PAGESZ, "incorrect segment id after split\n");

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment after split\n");

  ASSERT(o->segid == seg, "Bad segid field after split\n");
  ASSERT(o->asid == as, "Bad asid field after split\n");
  ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after split\n");
  ASSERT(o->address == (t_uint32)seg, "Bad address field after split\n");
  ASSERT(o->size == 2 * PAGESZ, "Bad size field after split\n");
  ASSERT(o->perms == PERM_READ, "Bad perms field after split\n");

  ASSERT(segment_get(seg2, &o) == ERROR_NONE, "error getting segment after split\n");

  ASSERT(o->segid == seg2, "Bad segid field after split\n");
  ASSERT(o->asid == as, "Bad asid field after split\n");
  ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after split\n");
  ASSERT(o->address == (t_uint32)seg2, "Bad address field after split\n");
  ASSERT(o->size == PAGESZ, "Bad size field after split\n");
  ASSERT(o->perms == PERM_READ, "Bad perms field after split\n");

  ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
