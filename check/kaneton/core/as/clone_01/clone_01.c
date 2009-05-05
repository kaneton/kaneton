/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/as/clone_01/clone_01.c
 *
 * created       matthieu bucchianeri   [fri feb 17 19:38:23 2006]
 * updated       matthieu bucchianeri   [sun may 27 20:27:06 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * address space cloning
 */

void		check_as_clone_01(void)
{
  i_task	task;
  i_task	forked;
  i_as		as;
  i_as		cloned;
  i_segment	seg;
  i_region	reg;
  o_as*		o;
  o_segment*	oseg;
  o_region*	oreg;
  t_setsz	sz;

  TEST_ENTER();

  assert(task_reserve(TASK_CLASS_GUEST,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &task) == ERROR_NONE,
	   "error creating task\n");

  assert(task_reserve(TASK_CLASS_GUEST,
			 TASK_BEHAV_INTERACTIVE,
			 TASK_PRIOR_INTERACTIVE,
			 &forked) == ERROR_NONE,
	   "error creating task\n");

  assert(as_reserve(task, &as) == ERROR_NONE, "error creating as\n");

  assert(segment_reserve(as,
			 3 * PAGESZ,
			 PERM_READ,
			 &seg) == ERROR_NONE,
	 "error reserving segment\n");

  assert(region_reserve(as,
			seg,
			PAGESZ,
			REGION_OPT_NONE,
			0,
			2 * PAGESZ,
			&reg) == ERROR_NONE,
	 "error reserving region\n");

  assert(as_clone(forked, as, &cloned) == ERROR_NONE, "as_clone failed\n");

  assert(as_get(cloned, &o) == ERROR_NONE, "invalid cloned as object\n");

  assert(set_size(o->segments, &sz) == ERROR_NONE, "invalid segment set in cloned object\n");

  assert(region_get(cloned, reg, &oreg) == ERROR_NONE, "invalid region set in cloned object\n");

  assert(segment_get(oreg->segid, &oseg) == ERROR_NONE, "invalid segment set in cloned object\n");

  assert(oseg->size == 3 * PAGESZ, "Bad size field in cloned segment\n");
  assert(oseg->perms == PERM_READ, "Bad perms field in cloned segment\n");
  assert(oseg->type == SEGMENT_TYPE_MEMORY, "Bad type field in cloned segment\n");

  assert(oreg->regid == reg, "Bad regid field in cloned region\n");
  assert(oreg->size == 2 * PAGESZ, "Bad size field in cloned region\n");
  assert(oreg->offset == PAGESZ, "Bad offset field in cloned region\n");
  assert(oreg->segid == oseg->segid, "Bad segid field in cloned region\n");

  assert(as_release(as) == ERROR_NONE,
	    "failed to release as\n");

  assert(task_release(task) == ERROR_NONE,
	    "failed to release task\n");

  assert(as_release(cloned) == ERROR_NONE,
	    "failed to release as\n");

  assert(task_release(forked) == ERROR_NONE,
	    "failed to release task\n");

  TEST_LEAVE();
}
