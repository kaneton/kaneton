/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...tests/kaneton/core/region/resize/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [mon nov 29 18:50:32 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

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

  if (segment_reserve(_kernel->as,
		      10 * PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel->as,
		     seg,
		     PAGESZ,
		     REGION_OPTION_NONE,
		     0,
		     2 * PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_resize(_kernel->as, reg, PAGESZ, &reg) != ERROR_OK)
    TEST_ERROR("[region_resize] error");

  if (region_get(_kernel->as, reg, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier after resize");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after resize");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after resize");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid region's size after resize");

  if (region_resize(_kernel->as, reg, 3 * PAGESZ, &reg) != ERROR_OK)
    TEST_ERROR("[region_resize] error");

  if (region_get(_kernel->as, reg, &o) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier after resize");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after resize");

  if (o->offset != PAGESZ)
    TEST_ERROR("invalid region's offset after resize");

  if (o->size != 3 * PAGESZ)
    TEST_ERROR("invalid region's size after resize");

  p = (t_uint8*)(t_vaddr)reg;
  for (; p < (t_uint8*)(t_vaddr)reg + 3 * PAGESZ; p++)
    {
      *p = 0x0d;

      if (*p != 0x0d)
	TEST_ERROR("the data read differs from the one written");
    }

  if (region_release(_kernel->as, reg) != ERROR_OK)
    TEST_ERROR("[region_release] error");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(ftr3wg0eeg90w2fg4);

  TEST_LEAVE();
}