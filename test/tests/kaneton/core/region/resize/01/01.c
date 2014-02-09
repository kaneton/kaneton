/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/core/region/resize/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:13:16 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_resize_01(void)
{
  i_segment		seg;
  i_region		reg;
  o_region*		o;
  t_uint8*		p;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      10 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
		     seg,
		     ___kaneton$pagesz,
		     REGION_OPTION_NONE,
		     0,
		     2 * ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_resize(_kernel.as, reg, ___kaneton$pagesz, &reg) != STATUS_OK)
    TEST_ERROR("[region_resize] error");

  if (region_get(_kernel.as, reg, &o) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  if (o->id != reg)
    TEST_ERROR("invalid region's identifier after resize");

  if (o->segment != seg)
    TEST_ERROR("invalid region's segment identifier after resize");

  if (o->offset != ___kaneton$pagesz)
    TEST_ERROR("invalid region's offset after resize");

  if (o->size != ___kaneton$pagesz)
    TEST_ERROR("invalid region's size after resize");

  p = (t_uint8*)o->address;
  for (; p < (t_uint8*)o->address + ___kaneton$pagesz; p++)
    {
      *p = 0x0d;

      if (*p != 0x0d)
	TEST_ERROR("the data read differs from the one written");
    }

  TEST_SIGNATURE(fw3ijwesf23t23t);

  TEST_LEAVE();
}
