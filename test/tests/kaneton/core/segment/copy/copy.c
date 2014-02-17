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
 * updated       julien quintard   [sun jan 30 21:09:21 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "copy.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_copy(void)
{
  i_segment		seg;
  i_segment		seg_ref;
  t_uint32		i;
  t_uint8		buff[___kaneton$pagesz];

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg_ref) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  for (i = 0; i < ___kaneton$pagesz; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  if (segment_write(seg_ref, 0, buff, ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[segment_write] error");

  if (segment_copy(seg, 0, seg_ref, 0,
		   ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[segment_copy] error");

  for (i = 0; i < ___kaneton$pagesz; i++)
    {
      buff[i] = 0;
    }

  if (segment_read(seg, 0, buff, ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[segment_read] error");

  for (i = 0; i < ___kaneton$pagesz; i++)
    {
      if (buff[i] != (i * 4 - 1) % 256)
	TEST_ERROR("invalid data in the segment");
    }

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg_ref) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(t9030awfoir4oj39t34);

  TEST_LEAVE();
}
