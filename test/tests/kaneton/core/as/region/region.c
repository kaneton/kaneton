/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/core/as/region/region.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri jan 14 22:56:36 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "region.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_region(void)
{
  i_task		task;
  i_as			as;
  int			i;
  i_segment		seg;
  i_region		reg;
  t_setsz		sz_before;
  t_setsz		sz_after;
  o_as*			o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (as_get(as, &o) != STATUS_OK)
    TEST_ERROR("[as_get] error");

  if (set_size(o->regions, &sz_before) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  for (i = 0; i < 10; i++)
    {
      if (segment_reserve(as,
			  ___kaneton$pagesz,
			  PERMISSION_READ | PERMISSION_WRITE,
			  SEGMENT_OPTION_NONE,
			  &seg) != STATUS_OK)
	TEST_ERROR("[segment_reserve] error");

      if (region_reserve(as,
			 seg,
			 0,
			 REGION_OPTION_NONE,
			 0,
			 ___kaneton$pagesz,
			 &reg) != STATUS_OK)
	TEST_ERROR("[region_reserve] error");
    }

  if (set_size(o->regions, &sz_after) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  if (sz_after < (sz_before + 10))
    TEST_ERROR("the regions have not been reserved properly");

  if (as_release(as) != STATUS_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task) != STATUS_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(r32iijogdpzfepwg0t);

  TEST_LEAVE();
}
