/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...ests/kaneton/core/region/reserve/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [mon nov 29 18:49:27 2010]
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

void			test_core_region_reserve_03(void)
{
  i_segment		seg;
  i_region		reg1;
  i_region		reg2;
  i_region		reg3;
  t_uint8*		p;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
                      10 * PAGESZ,
                      PERMISSION_READ | PERMISSION_WRITE,
                      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel->as,
                     seg,
                     0,
                     REGION_OPTION_NONE,
                     0,
                     2 * PAGESZ,
                     &reg1) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(_kernel->as,
                     seg,
                     3 * PAGESZ,
                     REGION_OPTION_NONE,
                     0,
                     2 * PAGESZ,
                     &reg2) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(_kernel->as,
                     seg,
                     6 * PAGESZ,
                     REGION_OPTION_NONE,
                     0,
                     4 * PAGESZ,
                     &reg3) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  p = (t_uint8*)(t_vaddr)reg1;
  for (; p < (t_uint8*)(t_vaddr)reg1 + 2 * PAGESZ; p++)
    *p = 0xaa;

  p = (t_uint8*)(t_vaddr)reg2;
  for (; p < (t_uint8*)(t_vaddr)reg2 + 2 * PAGESZ; p++)
    *p = 0x55;

  p = (t_uint8*)(t_vaddr)reg3;
  for (; p < (t_uint8*)(t_vaddr)reg3 + 4 * PAGESZ; p++)
    *p = 0x0;

  if (region_release(_kernel->as, reg1) != ERROR_OK)
    TEST_ERROR("[region_release] error");

  if (region_release(_kernel->as, reg2) != ERROR_OK)
    TEST_ERROR("[region_release] error");

  if (region_release(_kernel->as, reg3) != ERROR_OK)
    TEST_ERROR("[region_release] error");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(taf9ewi0g23r92tg);

  TEST_LEAVE();
}
