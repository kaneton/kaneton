/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/region/resize/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 13:27:56 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_region_resize_01(void)
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

  p = (t_uint8*)(t_vaddr)reg;
  for (; p < (t_uint8*)(t_vaddr)reg + PAGESZ; p++)
    {
      *p = 0x0d;

      if (*p != 0x0d)
	TEST_ERROR("the data read differs from the one written\n");
    }

  TEST_LEAVE();
}
