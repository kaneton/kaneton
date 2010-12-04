/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...architecture/ia32/region/inject/inject.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat dec  4 12:21:31 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "inject.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_region_inject(void)
{
  i_as			as;
  i_task		task;
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

  if (segment_reserve(_kernel->as,
		      10 * PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  o = malloc(sizeof (o_region));
  o->segment = seg;
  o->address = 0x40000000;
  o->offset = PAGESZ;
  o->size = 2 * PAGESZ;
  o->options = REGION_OPTION_FORCE;

  if (ia32_map_region(as,
		      seg,
		      PAGESZ,
		      REGION_OPTION_FORCE,
		      0x40000000,
		      2 * PAGESZ) != ERROR_OK)
    TEST_ERROR("[ia32_map_region] error");

  if (region_inject(as, o, &reg) != ERROR_OK)
    TEST_ERROR("[region_inject] error");

  if (region_get(as, reg, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset");

  if (o->size != 2 * PAGESZ)
    TEST_ERROR("invalid region's size");

  if (o->options != REGION_OPTION_FORCE)
    TEST_ERROR("invalid region's options");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  if (as_release(as) != ERROR_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task) != ERROR_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(w09ciekwijg4hg9);

  TEST_LEAVE();
}
