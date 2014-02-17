/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/core/segment/permissions/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:08:47 2011]
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

void			test_core_segment_permissions_05(void)
{
  i_segment		seg;
  i_segment		seg_ref;
  t_uint32		i;
  char			buff[___kaneton$pagesz];

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg_ref) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  for (i = 0; i < ___kaneton$pagesz; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  if (segment_write(seg_ref, 0, buff, ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[segment_write] error");

  if (segment_copy(seg, 0, seg_ref, 0, ___kaneton$pagesz) == STATUS_OK)
    TEST_ERROR("[segment_copy] error: allowed copy to a read only segment");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg_ref) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(riafe93gh09i4h);

  TEST_LEAVE();
}
