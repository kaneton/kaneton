/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../tests/kaneton/core/region/write/write.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:51:34 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "write.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_region_write(void)
{
  i_segment		seg;
  i_region		reg;
  t_uint8*		p;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel->as,
		     seg,
		     0,
		     REGION_OPTION_FORCE,
		     0x70000000,
		     ___kaneton$pagesz,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  p = (t_uint8*)(t_vaddr)reg;
  for (; p < (t_uint8*)(t_vaddr)reg + ___kaneton$pagesz; p++)
    *p = 0x42;

  TEST_SIGNATURE(0t09i490eirgi340g);

  TEST_LEAVE();
}
