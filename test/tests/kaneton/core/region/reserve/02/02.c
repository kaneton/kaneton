/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/core/region/reserve/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [wed nov 24 09:36:15 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_reserve_02(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_region		reg;
  o_region*		o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      2 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(as,
		     seg,
		     PAGESZ,
		     REGION_OPTION_FORCE,
		     0x40000000,
		     PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error\n");

  if (region_get(as, reg, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error\n");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier\n");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier\n");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset\n");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid region's size\n");

  if (as_release(as) != ERROR_OK)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_OK)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
