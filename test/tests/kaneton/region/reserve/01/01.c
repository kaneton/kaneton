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
 * updated       julien quintard   [mon nov  8 12:14:46 2010]
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
  i_region		reg1;
  i_region		reg2;
  i_region		reg3;
  t_uint8*		p;

  TEST_ENTER();

  if (segment_reserve(kasid,
                      10 * PAGESZ,
                      PERM_READ | PERM_WRITE,
                      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(kasid,
                     seg,
                     0,
                     REGION_OPT_NONE,
                     0,
                     2 * PAGESZ,
                     &reg1) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_reserve(kasid,
                     seg,
                     3 * PAGESZ,
                     REGION_OPT_NONE,
                     0,
                     2 * PAGESZ,
                     &reg2) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_reserve(kasid,
                     seg,
                     6 * PAGESZ,
                     REGION_OPT_NONE,
                     0,
                     4 * PAGESZ,
                     &reg3) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  p = (t_uint8*)(t_vaddr)reg1;
  for (; p < (t_uint8*)(t_vaddr)reg1 + 2 * PAGESZ; p++)
    *p = 0xaa;

  p = (t_uint8*)(t_vaddr)reg2;
  for (; p < (t_uint8*)(t_vaddr)reg2 + 2 * PAGESZ; p++)
    *p = 0x55;

  p = (t_uint8*)(t_vaddr)reg3;
  for (; p < (t_uint8*)(t_vaddr)reg3 + 4 * PAGESZ; p++)
    *p = 0x0;

  if (region_release(kasid, reg1) != ERROR_NONE)
    TEST_ERROR("[region_release] error\n");

  if (region_release(kasid, reg2) != ERROR_NONE)
    TEST_ERROR("[region_release] error\n");

  if (region_release(kasid, reg3) != ERROR_NONE)
    TEST_ERROR("[region_release] error\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
