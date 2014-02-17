/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/core/region/write/write.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:16:36 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "write.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_write(void)
{
  i_segment		seg;
  i_region		reg;
  t_uint8*		p;
  o_region*		r;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
		     seg,
		     0,
		     REGION_OPTION_FORCE,
		     0x70000000,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as, reg, &r) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  p = (t_uint8*)r->address;
  for (; p < (t_uint8*)r->address + ___kaneton$pagesz; p++)
    *p = 0x42;

  TEST_SIGNATURE(0t09i490eirgi340g);

  TEST_LEAVE();
}
