/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/resize_01/resize_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:47:54 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * resize.
 */

void		check_segment_resize_01(void)
{
  i_task	task;
  i_as	as;
  i_segment	seg;
  i_segment	seg2;
  i_segment	seg3;
  t_uint32	try = 0;
  o_segment*	o;

  TEST_ENTER();
  assert(task_reserve(TASK_CLASS_PROGRAM,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  assert(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  assert(segment_reserve(as,
			    3 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  assert(segment_resize(seg, PAGESZ, &seg) == ERROR_NONE,
	    "error resize\n");

  assert(segment_get(seg, &o) == ERROR_NONE, "error getting segment after resize\n");

  assert(o->segid == seg, "Bad segid field after resize\n");
  assert(o->asid == as, "Bad asid field after resize\n");
  assert(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after resize\n");
  assert(o->address == (t_uint32)seg, "Bad address field after resize\n");
  assert(o->size == PAGESZ, "Bad size field after resize\n");
  assert(o->perms == PERM_READ, "Bad perms field after resize\n");

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

  assert(segment_resize(seg, 10 * PAGESZ, &seg3) == ERROR_NONE,
	    "error resizing\n");

  assert(seg3 != seg, "some segments are overlapping\n");

  seg = seg3;

  assert(segment_get(seg, &o) == ERROR_NONE, "error getting segment after resize\n");

  assert(o->segid == seg, "Bad segid field after resize\n");
  assert(o->asid == as, "Bad asid field after resize\n");
  assert(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after resize\n");
  assert(o->address == (t_uint32)seg, "Bad address field after resize\n");
  assert(o->size == 10 * PAGESZ, "Bad size field after resize\n");
  assert(o->perms == PERM_READ, "Bad perms field after resize\n");

  assert(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  assert(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
