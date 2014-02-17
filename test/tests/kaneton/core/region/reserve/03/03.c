/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/core/region/reserve/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2030]
 * updated       julien quintard   [sun jan 30 21:16:25 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

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
  o_region*		r1;
  o_region*		r2;
  o_region*		r3;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
                      10 * ___kaneton$pagesz,
                      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
                      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
                     seg,
                     0,
                     REGION_OPTION_NONE,
                     0,
                     2 * ___kaneton$pagesz,
                     &reg1) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(_kernel.as,
                     seg,
                     3 * ___kaneton$pagesz,
                     REGION_OPTION_NONE,
                     0,
                     2 * ___kaneton$pagesz,
                     &reg2) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(_kernel.as,
                     seg,
                     6 * ___kaneton$pagesz,
                     REGION_OPTION_NONE,
                     0,
                     4 * ___kaneton$pagesz,
                     &reg3) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_get(_kernel.as, reg1, &r1) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  if (region_get(_kernel.as, reg2, &r2) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  if (region_get(_kernel.as, reg3, &r3) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  p = (t_uint8*)r1->address;
  for (; p < (t_uint8*)r1->address + 2 * ___kaneton$pagesz; p++)
    *p = 0xaa;

  p = (t_uint8*)r2->address;
  for (; p < (t_uint8*)r2->address + 2 * ___kaneton$pagesz; p++)
    *p = 0x55;

  p = (t_uint8*)r3->address;
  for (; p < (t_uint8*)r3->address + 4 * ___kaneton$pagesz; p++)
    *p = 0x0;

  if (region_release(_kernel.as, reg1) != STATUS_OK)
    TEST_ERROR("[region_release] error");

  if (region_release(_kernel.as, reg2) != STATUS_OK)
    TEST_ERROR("[region_release] error");

  if (region_release(_kernel.as, reg3) != STATUS_OK)
    TEST_ERROR("[region_release] error");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(taf9ewi0g23r92tg);

  TEST_LEAVE();
}
