/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/coalesce_01/coalesce_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:42:18 2007]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * coalesce.
 */

void		check_segment_coalesce_01(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_segid	seg2;
  t_uint32	try = 0;
  o_segment*	o;

  TEST_ENTER();
  ASSERT(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  while (try < 40)
    {
      ASSERT(segment_reserve(as,
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
    ASSERT(0, "FATAL ERROR\n");

  ASSERT(segment_coalesce(seg, seg2, &seg) == ERROR_NONE,
	    "error in coalesce\n");

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment after coalesce\n");

  ASSERT(o->segid == seg, "Bad segid field after coalesce\n");
  ASSERT(o->asid == as, "Bad asid field after coalesce\n");
  ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after coalesce\n");
  ASSERT(o->address == (t_uint32)seg, "Bad address field after coalesce\n");
  ASSERT(o->size == 2 * PAGESZ, "Bad size field after coalesce\n");
  ASSERT(o->perms == PERM_READ, "Bad perms field after coalesce\n");

  ASSERT(segment_get(seg2, &o) != ERROR_NONE, "old segment not removed after coalesce\n");

  ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  try = 0;
  while (try < 40)
    {
      ASSERT(segment_reserve(as,
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
    ASSERT(0, "FATAL ERROR\n");

  ASSERT(segment_perms(seg, PERM_WRITE) == ERROR_NONE,
	    "error setting perms\n");

  ASSERT(segment_coalesce(seg, seg2, &seg) != ERROR_NONE,
	    "coalesced segments with different perms\n");

  ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
