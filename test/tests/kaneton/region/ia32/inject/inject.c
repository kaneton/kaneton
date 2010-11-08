/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/region/ia32/inject/inject.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov  8 11:40:49 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "inject.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_region_ia32_inject(void)
{
  i_as			as;
  i_task		task;
  i_segment		seg;
  i_region		reg;
  o_region*		o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(kasid,
		      10 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  o = malloc(sizeof (o_region));
  o->segid = seg;
  o->address = 0x40000000;
  o->offset = PAGESZ;
  o->size = 2 * PAGESZ;
  o->opts = REGION_OPT_FORCE;

  if (ia32_map_region(as, seg,
		      PAGESZ, REGION_OPT_FORCE,
		      0x40000000, 2 * PAGESZ) != ERROR_NONE)
    TEST_ERROR("[ia32_map_region] error");

  if (region_inject(as, o, &reg) != ERROR_NONE)
    TEST_ERROR("[region_inject] error\n");

  if (region_get(as, reg, &o) != ERROR_NONE)
    TEST_ERROR("[region_get] error\n");

  if (o->regid != reg)
    TEST_ERROR("invalid region's identifier\n");

  if (o->segid != seg)
    TEST_ERROR("invalid region's segment identifier\n");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset\n");

  if (o->size != 2 * PAGESZ)
    TEST_ERROR("invalid region's size\n");

  if (o->opts != REGION_OPT_FORCE)
    TEST_ERROR("invalid region's options\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
