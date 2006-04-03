/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/09/09.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [mon apr  3 14:52:27 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * split.
 */

void		check_segment_09(void)
{
  t_tskid	task;
  t_asid	as;
  t_segid	seg;
  t_segid	seg2;
  o_segment*	o;

  TEST_ENTER;
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    3 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(segment_split(seg, 2 * PAGESZ, &seg, &seg2) == ERROR_NONE,
	    "error splitting\n");

  MY_ASSERT(seg2 == seg + 2 * PAGESZ, "incorrect segment id\n");

  MY_ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment\n");

  MY_ASSERT(o->segid == seg, "Bad segid field\n");
  MY_ASSERT(o->asid == as, "Bad asid field\n");
  MY_ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field\n");
  MY_ASSERT(o->address == (t_uint32)seg, "Bad address field\n");
  MY_ASSERT(o->size == 2 * PAGESZ, "Bad size field\n");
  MY_ASSERT(o->perms == PERM_READ, "Bad perms field\n");

  MY_ASSERT(segment_get(seg2, &o) == ERROR_NONE, "error getting segment\n");

  MY_ASSERT(o->segid == seg2, "Bad segid field\n");
  MY_ASSERT(o->asid == as, "Bad asid field\n");
  MY_ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field\n");
  MY_ASSERT(o->address == (t_uint32)seg2, "Bad address field\n");
  MY_ASSERT(o->size == PAGESZ, "Bad size field\n");
  MY_ASSERT(o->perms == PERM_READ, "Bad perms field\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
