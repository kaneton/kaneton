/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/10/10.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [thu sep  7 23:37:21 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * resize.
 */

void		check_segment_10(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_segid	seg2;
  t_segid	seg3;
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
			    3 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  ASSERT(segment_resize(seg, PAGESZ, &seg) == ERROR_NONE,
	    "error resize\n");

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment after resize\n");

  ASSERT(o->segid == seg, "Bad segid field after resize\n");
  ASSERT(o->asid == as, "Bad asid field after resize\n");
  ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after resize\n");
  ASSERT(o->address == (t_uint32)seg, "Bad address field after resize\n");
  ASSERT(o->size == PAGESZ, "Bad size field after resize\n");
  ASSERT(o->perms == PERM_READ, "Bad perms field after resize\n");

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

  ASSERT(segment_resize(seg, 10 * PAGESZ, &seg3) == ERROR_NONE,
	    "error resizing\n");

  ASSERT(seg3 != seg, "some segments are overlapping\n");

  seg = seg3;

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting segment after resize\n");

  ASSERT(o->segid == seg, "Bad segid field after resize\n");
  ASSERT(o->asid == as, "Bad asid field after resize\n");
  ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after resize\n");
  ASSERT(o->address == (t_uint32)seg, "Bad address field after resize\n");
  ASSERT(o->size == 10 * PAGESZ, "Bad size field after resize\n");
  ASSERT(o->perms == PERM_READ, "Bad perms field after resize\n");

  ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
