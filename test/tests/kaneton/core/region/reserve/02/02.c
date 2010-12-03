/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...ests/kaneton/core/region/reserve/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [mon nov 29 18:49:07 2010]
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
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (segment_reserve(as,
		      2 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
		     seg,
		     PAGESZ,
		     REGION_OPTION_FORCE,
		     0x40000000,
		     PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(as, reg, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid region's size");

  if (as_release(as) != ERROR_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task) != ERROR_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(0923i0f9weaaaaawe392);

  TEST_LEAVE();
}
