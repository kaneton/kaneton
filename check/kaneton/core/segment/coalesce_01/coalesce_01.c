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

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * coalesce.
 */

void		check_segment_coalesce_01(void)
{
  i_task	task;
  i_as	as;
  i_segment	seg;
  i_segment	seg2;
  t_uint32	try = 0;
  o_segment*	o;

  TEST_ENTER();
  assert(task_reserve(TASK_CLASS_GUEST,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  assert(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  assert(segment_reserve(as,
			    PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  while (try < 40)
    {
      assert(segment_reserve(as,
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
    assert(0, "FATAL ERROR\n");

  assert(segment_coalesce(seg, seg2, &seg) == ERROR_NONE,
	    "error in coalesce\n");

  assert(segment_get(seg, &o) == ERROR_NONE, "error getting segment after coalesce\n");

  assert(o->segid == seg, "Bad segid field after coalesce\n");
  assert(o->asid == as, "Bad asid field after coalesce\n");
  assert(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after coalesce\n");
  assert(o->address == (t_uint32)seg, "Bad address field after coalesce\n");
  assert(o->size == 2 * PAGESZ, "Bad size field after coalesce\n");
  assert(o->perms == PERM_READ, "Bad perms field after coalesce\n");

  assert(segment_get(seg2, &o) != ERROR_NONE, "old segment not removed after coalesce\n");

  assert(segment_reserve(as,
			    PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  try = 0;
  while (try < 40)
    {
      assert(segment_reserve(as,
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
    assert(0, "FATAL ERROR\n");

  assert(segment_perms(seg, PERM_WRITE) == ERROR_NONE,
	    "error setting perms\n");

  assert(segment_coalesce(seg, seg2, &seg) != ERROR_NONE,
	    "coalesced segments with different perms\n");

  assert(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  assert(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
