/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...aneton/core/as/translation/translation.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:43:37 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "translation.h"

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_translation(void)
{
  i_segment		seg;
  i_region		reg;
  t_uint32		i;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      2048 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel->as,
		     seg,
		     4 * PAGESZ,
		     REGION_OPTION_NONE,
		     0,
		     8 * PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  /*
   * virtual to physical
   */

  PHYSICAL((t_vaddr)reg,
	   (t_paddr)seg + 4 * PAGESZ);

  PHYSICAL((t_vaddr)reg + 1,
	   (t_paddr)seg + 4 * PAGESZ + 1);

  PHYSICAL((t_vaddr)reg + 2 * PAGESZ + 1234,
	   (t_paddr)seg + 6 * PAGESZ + 1234);

  for (i = 0; i < 8 * PAGESZ; i++)
    PHYSICAL((t_vaddr)reg + i, (t_paddr)seg + 4 * PAGESZ + i);

  /*
   * physical to virtual
   */

  VIRTUAL((t_paddr)seg + 4 * PAGESZ,
	  (t_vaddr)reg);

  VIRTUAL((t_paddr)seg + 4 * PAGESZ + 1,
	  (t_vaddr)reg + 1);

  VIRTUAL((t_paddr)seg + 6 * PAGESZ + 1234,
	  (t_vaddr)reg + 2 * PAGESZ + 1234);

  for (i = 0; i < 8 * PAGESZ; i++)
    VIRTUAL((t_paddr)seg + 4 * PAGESZ + i,
	    (t_vaddr)reg + i);

  TEST_SIGNATURE(r3289u9idsjjsfwe9tg9);

  TEST_LEAVE();
}
