/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...E/test/tests/kaneton/segment/copy/copy.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 14:26:53 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "copy.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_copy(void)
{
  i_segment		seg;
  i_segment		seg_ref;
  t_uint32		i;
  t_uint8		buff[PAGESZ];

  TEST_ENTER();

  if (segment_reserve(kasid,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg_ref) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(kasid,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  if (segment_write(seg_ref, 0, buff, PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_write] error\n");

  if (segment_copy(seg, 0, seg_ref, 0,
		   PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_copy] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  if (segment_read(seg, 0, buff, PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_read] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      if (buff[i] != (i * 4 - 1) % 256)
	TEST_ERROR("invalid data in the segment\n");
    }

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg_ref) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
