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
 * updated       julien quintard   [thu nov 18 16:14:33 2010]
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
		   &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error\n");

  if (as_get(as, &o) != ERROR_OK)
    TEST_ERROR("[as_get] error\n");

  if (set_size(o->regions, &sz_before) != ERROR_OK)
    TEST_ERROR("[set_size] error\n");

  for (i = 0; i < 10; i++)
    {
      if (segment_reserve(as,
			  PAGESZ,
			  PERMISSION_READ | PERMISSION_WRITE,
			  &seg) != ERROR_OK)
	TEST_ERROR("[segment_reserve] error\n");

      if (region_reserve(as,
			 seg,
			 0,
			 REGION_OPTION_NONE,
			 0,
			 PAGESZ,
			 &reg) != ERROR_OK)
	TEST_ERROR("[region_reserve] error\n");
    }

  if (set_size(o->regions, &sz_after) != ERROR_OK)
    TEST_ERROR("[set_size] error\n");

  if (sz_after < (sz_before + 10))
    TEST_ERROR("the regions have not been reserved properly\n");

  if (as_release(as) != ERROR_OK)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_OK)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
