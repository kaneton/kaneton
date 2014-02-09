/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/core/as/translation/translation.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:13:02 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "translation.h"

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_translation(void)
{
  i_segment		seg;
  i_region		reg;
  o_segment*		segment;
  o_region*		region;
  t_uint32		i;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      2048 * ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel.as,
		     seg,
		     4 * ___kaneton$pagesz,
		     REGION_OPTION_NONE,
		     0,
		     8 * ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (segment_get(seg, &segment) != STATUS_OK)
    TEST_ERROR("[segment_get] error");

  if (region_get(_kernel.as, reg, &region) != STATUS_OK)
    TEST_ERROR("[region_get] error");

  /*
   * virtual to physical
   */

  PHYSICAL(region->address,
	   segment->address + 4 * ___kaneton$pagesz);

  PHYSICAL(region->address + 1,
	   segment->address + 4 * ___kaneton$pagesz + 1);

  PHYSICAL(region->address + 2 * ___kaneton$pagesz + 1234,
	   segment->address + 6 * ___kaneton$pagesz + 1234);

  for (i = 0; i < 8 * ___kaneton$pagesz; i++)
    PHYSICAL(region->address + i,
	     segment->address + 4 * ___kaneton$pagesz + i);

  /*
   * physical to virtual
   */

  VIRTUAL(segment->address + 4 * ___kaneton$pagesz,
	  region->address);

  VIRTUAL(segment->address + 4 * ___kaneton$pagesz + 1,
	  region->address + 1);

  VIRTUAL(segment->address + 6 * ___kaneton$pagesz + 1234,
	  region->address + 2 * ___kaneton$pagesz + 1234);

  for (i = 0; i < 8 * ___kaneton$pagesz; i++)
    VIRTUAL(segment->address + 4 * ___kaneton$pagesz + i,
	    region->address + i);

  TEST_SIGNATURE(r3289u9idsjjsfwe9tg9);

  TEST_LEAVE();
}
