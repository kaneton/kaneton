/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/core/segment/permissions/04/04.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:08:40 2011]
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

void			test_core_segment_permissions_04(void)
{
  i_segment		seg;
  t_uint32		i = 0x41424344;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_write(seg, 0, &i, sizeof (t_uint32)) == STATUS_OK)
    TEST_ERROR("[segment_write] error: allowed writing to a read "
	       "only segment\n");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(t3oarfskr34h90);

  TEST_LEAVE();
}
