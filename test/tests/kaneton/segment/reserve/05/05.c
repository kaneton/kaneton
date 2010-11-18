/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/segment/reserve/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 15:40:00 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_reserve_05(void)
{
  i_segment		seg[4];

  TEST_ENTER();

  TEST_ALLOCATE(kasid, 1, seg);
  TEST_ALLOCATE(kasid, 9, seg + 1);
  TEST_ALLOCATE(kasid, 3, seg + 2);
  TEST_ALLOCATE(kasid, 7, seg + 1);
  TEST_ALLOCATE(kasid, 8, seg);
  TEST_ALLOCATE(kasid, 2, seg + 3);

  TEST_LEAVE();
}
