/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/segment/read-write/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 14:42:38 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_readwrite_02(void)
{
  i_segment		seg;
  char			buff[2 * PAGESZ];

  TEST_ENTER();

  if (segment_reserve(kasid, PAGESZ, PERM_READ | PERM_WRITE, &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_read(seg, 2 * PAGESZ, buff, sizeof(t_uint32)) == ERROR_NONE)
    TEST_ERROR("[segment_read] error: out of bound\n");

  if (segment_read(seg, 0, buff, 2 * PAGESZ) == ERROR_NONE)
    TEST_ERROR("[segment_read] error: overflow\n");

  if (segment_write(seg, 2 * PAGESZ, buff, sizeof(t_uint32)) == ERROR_NONE)
    TEST_ERROR("[segment_write] error: out of bound\n");

  if (segment_write(seg, 0, buff, 2 * PAGESZ) == ERROR_NONE)
    TEST_ERROR("[segment_write] error: overflow\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
