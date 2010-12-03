/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...kaneton/core/segment/permissions/05/05.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:57:18 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "05.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_permissions_05(void)
{
  i_segment		seg;
  i_segment		seg_ref;
  t_uint32		i;
  char			buff[PAGESZ];

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg_ref) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  if (segment_write(seg_ref, 0, buff, PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_write] error");

  if (segment_copy(seg, 0, seg_ref, 0, PAGESZ) == ERROR_OK)
    TEST_ERROR("[segment_copy] error: allowed copy to a read only segment");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg_ref) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(riafe93gh09i4h);

  TEST_LEAVE();
}
