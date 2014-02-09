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
 * updated       julien quintard   [fri jan 14 23:03:25 2011]
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
		   &task) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (segment_reserve(as,
		      2 * ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
		     seg,
		     ___kaneton$pagesz,
		     REGION_OPTION_FORCE,
		     0x40000000,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(as, reg, &o) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier");

  if (o->offset != ___kaneton$pagesz)
    TEST_ERROR("invalid region's offset");

  if (o->size != ___kaneton$pagesz)
    TEST_ERROR("invalid region's size");

  if (as_release(as) != STATUS_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task) != STATUS_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(0923i0f9weaaaaawe392);

  TEST_LEAVE();
}
