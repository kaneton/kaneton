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

#include <libc/libc.h>
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
  assert(task_reserve(TASK_CLASS_GUEST,

			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  assert(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  assert(segment_reserve(as,
			    2 * PAGESZ,
			    PERM_READ | PERM_EXEC,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  assert(segment_perms(seg, PERM_READ | PERM_WRITE) == ERROR_NONE,
	    "error setting perms\n");

  assert(segment_type(seg, SEGMENT_TYPE_CATCH) == ERROR_NONE,
	    "error setting type\n");

  assert(segment_get(seg, &o) == ERROR_NONE, "error getting as\n");

  assert(o->perms == (PERM_READ | PERM_WRITE), "Bad perm field after changing perms\n");
  assert(o->type == SEGMENT_TYPE_CATCH, "Bad type field after changing type\n");

  assert(segment_perms(seg, ~(PERM_READ | PERM_WRITE | PERM_EXEC)) !=
	    ERROR_NONE,
	    "error setting bad perms\n");

  assert(segment_type(seg, (1 << 4)) != ERROR_NONE,
	    "error setting bad type\n");

  assert(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  assert(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
