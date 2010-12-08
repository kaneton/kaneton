/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../kaneton/core/segment/read-write/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:58:16 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_readwrite_02(void)
{
  i_segment		seg;
  char			buff[2 * PAGESZ];

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_read(seg, 2 * PAGESZ, buff, sizeof(t_uint32)) == ERROR_OK)
    TEST_ERROR("[segment_read] error: out of bound");

  if (segment_read(seg, 0, buff, 2 * PAGESZ) == ERROR_OK)
    TEST_ERROR("[segment_read] error: overflow");

  if (segment_write(seg, 2 * PAGESZ, buff, sizeof(t_uint32)) == ERROR_OK)
    TEST_ERROR("[segment_write] error: out of bound");

  if (segment_write(seg, 0, buff, 2 * PAGESZ) == ERROR_OK)
    TEST_ERROR("[segment_write] error: overflow");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(0r320afkeoaweigj3g43);

  TEST_LEAVE();
}