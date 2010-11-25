/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/core/segment/copy/copy.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu nov 18 16:25:24 2010]
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
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  if (segment_write(seg_ref, 0, buff, PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_write] error\n");

  if (segment_copy(seg, 0, seg_ref, 0,
		   PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_copy] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  if (segment_read(seg, 0, buff, PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_read] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      if (buff[i] != (i * 4 - 1) % 256)
	TEST_ERROR("invalid data in the segment\n");
    }

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg_ref) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
