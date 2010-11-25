/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/core/region/flush/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [thu nov 18 16:19:13 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_flush_02(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_region		reg;
  t_uint32		i;
  o_as*			o;
  t_setsz		sz;

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

  if (region_flush(as) != ERROR_OK)
    TEST_ERROR("[region_flush] error\n");

  if (set_size(o->regions, &sz) != ERROR_OK)
    TEST_ERROR("[set_size] error\n");

  if (sz != 0)
    TEST_ERROR("there should not be regions left after a flush\n");

  if (as_release(as) != ERROR_OK)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_OK)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
