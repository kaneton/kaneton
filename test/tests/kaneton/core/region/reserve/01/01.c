/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/region/reserve/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 07:30:36 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- extern ----------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_region_reserve_01(void)
{
  i_segment		seg;
  i_region		reg;
  o_region*		o;

  TEST_ENTER();

  if (segment_reserve(kasid,
		      2 * PAGESZ,
		      PERM_READ,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(kasid,
		     seg,
		     0,
		     REGION_OPT_NONE,
		     0,
		     2 * PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_get(kasid, reg, &o) != ERROR_NONE)
    TEST_ERROR("[region_get] error\n");

  if (o->regid != reg)
    TEST_ERROR("invalid region's identifier\n");

  if (o->segid != seg)
    TEST_ERROR("invalid region's segment identifier\n");

  if (o->offset != 0)
    TEST_ERROR("invalid region's offset\n");

  if (o->size != 2 * PAGESZ)
    TEST_ERROR("invalid region's size\n");

  TEST_LEAVE();
}
