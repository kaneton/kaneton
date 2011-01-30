/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/segment/reserve/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:07:50 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_reserve_05(void)
{
  i_segment		seg[4];

  TEST_ENTER();

  TEST_ALLOCATE(_kernel.as, 1, seg);
  TEST_ALLOCATE(_kernel.as, 9, seg + 1);
  TEST_ALLOCATE(_kernel.as, 3, seg + 2);
  TEST_ALLOCATE(_kernel.as, 7, seg + 1);
  TEST_ALLOCATE(_kernel.as, 8, seg);
  TEST_ALLOCATE(_kernel.as, 2, seg + 3);

  TEST_SIGNATURE(r223t9it9585hwe);

  TEST_LEAVE();
}
