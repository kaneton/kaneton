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

void		check_as_clone_02(void)
{
  i_task	task;
  i_task	forked;
  i_as		as;
  i_as		cloned;
  i_segment	seg;
  i_region	reg;
  o_as*		o;
  o_segment*	osegs;
  o_segment*	osegc;
  o_region*	oregs;
  o_region*	oregc;
  i_region*	ireg;
  t_iterator	it;
  t_state	state;
  t_setsz	sz1;
  t_setsz	sz2;

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

  assert(set_size(o->regions, &sz1) == ERROR_NONE, "error in set_size\n");

  assert(as_get(as, &o) == ERROR_NONE, "cannot get as object\n");

  assert(set_size(o->regions, &sz2) == ERROR_NONE, "error in set_size\n");

  assert(sz1 == sz2, "Different number of regions in cloned as\n");

  set_foreach(SET_OPT_FORWARD, o->regions, &it, state)
    {
      assert(set_object(o->regions, it, (void**)&ireg) == ERROR_NONE,
	     "error getting region object\n");

      assert(region_get(as, *ireg, &oregs) == ERROR_NONE,
	     "error getting region object\n");
      assert(region_get(cloned, *ireg, &oregc) == ERROR_NONE,
	     "region not present in cloned as\n");

      assert(oregs->address == oregc->address,
	     "Bad address field in cloned region\n");
      assert(oregs->offset == oregc->offset,
	     "Bad offset field in cloned region\n");
      assert(oregs->size == oregc->size,
	     "Bad size field in cloned region\n");

      assert(segment_get(oregs->segid, &osegs) == ERROR_NONE,
	     "error getting segment object\n");
      assert(segment_get(oregc->segid, &osegc) == ERROR_NONE,
	     "error getting segment object\n");

      assert(osegs->type == osegc->type,
	     "Bad type field in cloned segment\n");
      assert(osegs->size == osegc->size,
	     "Bad size field in cloned segment\n");
      assert(osegs->perms == osegc->perms,
	     "Bad perms field in cloned segment\n");
    }

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
