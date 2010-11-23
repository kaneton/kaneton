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
 * updated       julien quintard   [thu nov 18 16:31:02 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "04.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_reserve_04(void)
{
  i_segment		seg[4];

  TEST_ENTER();

  TEST_ALLOCATE(kasid, 2, seg);
  TEST_ALLOCATE(kasid, 8, seg + 1);
  TEST_ALLOCATE(kasid, 2, seg + 2);
  TEST_ALLOCATE(kasid, 5, seg + 1);
  TEST_ALLOCATE(kasid, 8, seg);
  TEST_ALLOCATE(kasid, 4, seg + 3);

  TEST_LEAVE();
}
