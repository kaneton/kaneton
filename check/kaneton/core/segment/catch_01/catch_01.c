/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/catch_01/catch_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:37:18 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * segment_catch.
 */

void		check_segment_catch_01(void)
{
  i_task	task1;
  i_task	task2;
  i_as	as1;
  i_as	as2;
  i_segment	seg;
  o_segment*	o;

  TEST_ENTER();
  assert(task_reserve(TASK_CLASS_GUEST,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task1) == ERROR_NONE,
	   "error creating task\n");

  assert(task_reserve(TASK_CLASS_GUEST,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task2) == ERROR_NONE,
	   "error creating task\n");

  assert(as_reserve(task1, &as1) == ERROR_NONE, "error creating as\n");

  assert(as_reserve(task2, &as2) == ERROR_NONE, "error creating as\n");

  assert(segment_reserve(as1,
			    2 * PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  assert(segment_type(seg, SEGMENT_TYPE_CATCH) == ERROR_NONE,
	    "cannot set a segment as catcheable\n");

  assert(segment_catch(as2, seg) == ERROR_NONE,
	    "cannot catch the segment\n");

  assert(segment_get(seg, &o) == ERROR_NONE, "error getting as\n");

  assert(o->asid = as2, "Bad type field after catch\n");
  assert(o->type = SEGMENT_TYPE_MEMORY, "Bad type field aftet catch\n");

  assert(as_release(as1) == ERROR_NONE,
	    "failed to release as\n");

  assert(as_release(as2) == ERROR_NONE,
	    "failed to release as\n");

  assert(task_release(task1) == ERROR_NONE,
	    "failed to release task\n");

  assert(task_release(task2) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
