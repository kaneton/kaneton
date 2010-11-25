/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/core/as/clone/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [wed nov 24 09:25:27 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_clone_02(void)
{
  i_task		task;
  i_task		forked;
  i_as			as;
  i_as			cloned;
  i_segment		seg;
  i_region		reg;
  o_as*			o;
  o_segment*		osegs;
  o_segment*		osegc;
  o_region*		oregs;
  o_region*		oregc;
  i_region*		ireg;
  t_iterator		it;
  t_state		state;
  t_setsz		sz1;
  t_setsz		sz2;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error\n");

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &forked) != ERROR_OK)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      3 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(as,
		     seg,
		     PAGESZ,
		     REGION_OPTION_NONE,
		     0,
		     2 * PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error\n");

  if (as_clone(forked, as, &cloned) != ERROR_OK)
    TEST_ERROR("[as_clone] error\n");

  if (as_get(cloned, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error: unable to retrieve the cloned "
	       "address space\n");

  if (set_size(o->regions, &sz1) != ERROR_OK)
    TEST_ERROR("[set_size] error\n");

  if (as_get(as, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error\n");

  if (set_size(o->regions, &sz2) != ERROR_OK)
    TEST_ERROR("[set_size] error\n");

  if (sz1 != sz2)
    TEST_ERROR("the original and cloned address spaces have a different "
	       "number of regions");

  set_foreach(SET_OPTION_FORWARD, o->regions, &it, state)
    {
      if (set_object(o->regions, it, (void**)&ireg) != ERROR_OK)
	TEST_ERROR("[set_object] error\n");

      if (region_get(as, *ireg, &oregs) != ERROR_OK)
	TEST_ERROR("[region_get] error\n");

      if (region_get(cloned, *ireg, &oregc) != ERROR_OK)
	TEST_ERROR("[region_get] error: unable to retrieve a region from "
		   "the cloned address space\n");

      if (oregs->address != oregc->address)
	TEST_ERROR("two supposely identical regions from cloned address "
		   "spaces have different addresses");

      if (oregs->offset != oregc->offset)
	TEST_ERROR("two supposely identical regions from cloned address "
		   "spaces have different offsets");

      if (oregs->size != oregc->size)
	TEST_ERROR("two supposely identical regions from cloned address "
		   "spaces have different sizes");

      if (segment_get(oregs->segment, &osegs) != ERROR_OK)
	TEST_ERROR("[segment_get] error\n");

      if (segment_get(oregc->segment, &osegc) != ERROR_OK)
	TEST_ERROR("[segment_get] error: unable to retrieve a segment from "
		   "the cloned address space\n");

      if (osegs->type != osegc->type)
	TEST_ERROR("two supposely identical segments from cloned address "
		   "spaces have different types");

      if (osegs->size != osegc->size)
	TEST_ERROR("two supposely identical segments from cloned address "
		   "spaces have different sizes");

      if (osegs->permissions != osegc->permissions)
	TEST_ERROR("two supposely identical segments from cloned address "
		   "spaces have different permissions");
    }

  if (as_release(as) != ERROR_OK)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_OK)
    TEST_ERROR("[task_release] error\n");

  if (as_release(cloned) != ERROR_OK)
    TEST_ERROR("[as_release] error\n");

  if (task_release(forked) != ERROR_OK)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
