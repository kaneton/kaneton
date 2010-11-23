/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/core/region/resize/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [thu nov 18 16:22:25 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_resize_03(void)
{
  i_segment		seg;
  i_region		reg;
  o_region*		o;
  t_uint8*		p;

  TEST_ENTER();

  if (segment_reserve(kasid,
		      10 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(kasid,
		     seg,
		     PAGESZ,
		     REGION_OPT_NONE,
		     0,
		     2 * PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_resize(kasid, reg, PAGESZ, &reg) != ERROR_NONE)
    TEST_ERROR("[region_resize] error\n");

  if (region_get(kasid, reg, &o) != ERROR_NONE)
    TEST_ERROR("[region_get] error\n");

  if (o->regid != reg)
    TEST_ERROR("invalid region's identifier after resize\n");

  if (o->segid != seg)
    TEST_ERROR("invalid region's segment identifier after resize\n");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after resize\n");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid region's size after resize\n");

  if (region_resize(kasid, reg, 3 * PAGESZ, &reg) != ERROR_NONE)
    TEST_ERROR("[region_resize] error\n");

  if (region_get(kasid, reg, &o) != ERROR_NONE)
    TEST_ERROR("[region_get] error\n");

  if (o->regid != reg)
    TEST_ERROR("invalid region's identifier after resize\n");

  if (o->segid != seg)
    TEST_ERROR("invalid region's segment identifier after resize\n");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after resize\n");

  if (o->size != 3 * PAGESZ)
    TEST_ERROR("invalid region's size after resize\n");

  p = (t_uint8*)(t_vaddr)reg;
  for (; p < (t_uint8*)(t_vaddr)reg + 3 * PAGESZ; p++)
    {
      *p = 0x0d;

      if (*p != 0x0d)
	TEST_ERROR("the data read differs from the one written\n");
    }

  if (region_release(kasid, reg) != ERROR_NONE)
    TEST_ERROR("[region_release] error\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
