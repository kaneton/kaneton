/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/segment/03/03.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [thu sep  7 23:33:30 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * type & perms of segments.
 */

void		check_segment_03(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  o_segment*	o;

  TEST_ENTER;
  MY_ASSERT(task_reserve(TASK_CLASS_PROGRAM,

			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  MY_ASSERT(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  MY_ASSERT(segment_reserve(as,
			    2 * PAGESZ,
			    PERM_READ | PERM_EXEC,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  MY_ASSERT(segment_perms(seg, PERM_READ | PERM_WRITE) == ERROR_NONE,
	    "error setting perms\n");

  MY_ASSERT(segment_type(seg, SEGMENT_TYPE_CATCH) == ERROR_NONE,
	    "error setting type\n");

  MY_ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting as\n");

  MY_ASSERT(o->perms == (PERM_READ | PERM_WRITE), "Bad perm field after changing perms\n");
  MY_ASSERT(o->type == SEGMENT_TYPE_CATCH, "Bad type field after changing type\n");

  MY_ASSERT(segment_perms(seg, ~(PERM_READ | PERM_WRITE | PERM_EXEC)) !=
	    ERROR_NONE,
	    "error setting bad perms\n");

  MY_ASSERT(segment_type(seg, (1 << 4)) != ERROR_NONE,
	    "error setting bad type\n");

  MY_ASSERT(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  MY_ASSERT(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE;
}
