/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...t/tests/kaneton/core/segment/copy/copy.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:53:53 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "copy.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_copy(void)
{
  i_segment		seg;
  i_segment		seg_ref;
  t_uint32		i;
  t_uint8		buff[PAGESZ];

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg_ref) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  if (segment_write(seg_ref, 0, buff, PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_write] error");

  if (segment_copy(seg, 0, seg_ref, 0,
		   PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_copy] error");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  if (segment_read(seg, 0, buff, PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_read] error");

  for (i = 0; i < PAGESZ; i++)
    {
      if (buff[i] != (i * 4 - 1) % 256)
	TEST_ERROR("invalid data in the segment");
    }

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg_ref) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(t9030awfoir4oj39t34);

  TEST_LEAVE();
}
