/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/type_01/type_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:51:45 2007]
 */

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * type & perms of segments.
 */

void		check_segment_type_01(void)
{
  i_task	task;
  i_as	as;
  i_segment	seg;
  o_segment*	o;

  TEST_ENTER();
  ASSERT(task_reserve(TASK_CLASS_PROGRAM,

			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  ASSERT(segment_reserve(as,
			    2 * PAGESZ,
			    PERM_READ | PERM_EXEC,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  ASSERT(segment_perms(seg, PERM_READ | PERM_WRITE) == ERROR_NONE,
	    "error setting perms\n");

  ASSERT(segment_type(seg, SEGMENT_TYPE_CATCH) == ERROR_NONE,
	    "error setting type\n");

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting as\n");

  ASSERT(o->perms == (PERM_READ | PERM_WRITE), "Bad perm field after changing perms\n");
  ASSERT(o->type == SEGMENT_TYPE_CATCH, "Bad type field after changing type\n");

  ASSERT(segment_perms(seg, ~(PERM_READ | PERM_WRITE | PERM_EXEC)) !=
	    ERROR_NONE,
	    "error setting bad perms\n");

  ASSERT(segment_type(seg, (1 << 4)) != ERROR_NONE,
	    "error setting bad type\n");

  ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
