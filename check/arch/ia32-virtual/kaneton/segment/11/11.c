/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/segment/11/11.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun apr  2 17:59:17 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * coalesce.
 */

void		check_segment_11(void)
{
  t_tskid	task;
  t_asid	as;
  t_segid	seg;
  t_segid	seg2;
  t_uint32	try = 0;
  o_segment*	o;

  TEST_ENTER;
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  while (try < 40)
    {
      MY_ASSERT(segment_reserve(as,
				PAGESZ,
				PERM_READ,
				&seg2) == ERROR_NONE,
		"error reserving segment\n");

      if (seg2 == seg + PAGESZ)
	break;

      seg = seg2;

      try++;
    }

  if (try == 40)
    MY_ASSERT(0, "FATAL ERROR\n");

  MY_ASSERT(segment_coalesce(seg, seg2, &seg) == ERROR_NONE,
	    "error in coalesce\n");

  MY_ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment\n");

  MY_ASSERT(o->segid == seg, "Bad segid field\n");
  MY_ASSERT(o->asid == as, "Bad asid field\n");
  MY_ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field\n");
  MY_ASSERT(o->address == (t_uint32)seg, "Bad address field\n");
  MY_ASSERT(o->size == 2 * PAGESZ, "Bad size field\n");
  MY_ASSERT(o->perms == PERM_READ, "Bad perms field\n");

  MY_ASSERT(segment_get(seg2, &o) != ERROR_NONE, "old segment not removed\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
