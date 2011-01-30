/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/segment/reserve/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:07:42 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "04.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_reserve_04(void)
{
  i_segment		seg[4];

  TEST_ENTER();

  TEST_ALLOCATE(_kernel.as, 2, seg);
  TEST_ALLOCATE(_kernel.as, 8, seg + 1);
  TEST_ALLOCATE(_kernel.as, 2, seg + 2);
  TEST_ALLOCATE(_kernel.as, 5, seg + 1);
  TEST_ALLOCATE(_kernel.as, 8, seg);
  TEST_ALLOCATE(_kernel.as, 4, seg + 3);

  TEST_SIGNATURE(32u98f3we4g3);

  TEST_LEAVE();
}
