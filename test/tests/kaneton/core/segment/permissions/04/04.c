/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/segment/permissions/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 14:39:23 2010]
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

void			test_segment_permissions_04(void)
{
  i_segment		seg;
  t_uint32		i = 0x41424344;

  TEST_ENTER();

  if (segment_reserve(kasid, PAGESZ, PERM_READ, &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_write(seg, 0, &i, sizeof(t_uint32)) == ERROR_NONE)
    TEST_ERROR("[segment_write] error: allowed writing to a read "
	       "only segment\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
