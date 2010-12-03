/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../kaneton/core/segment/read-write/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:57:49 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_readwrite_01(void)
{
  i_segment		seg;
  t_uint32		i;
  t_uint8		buff[PAGESZ];

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  for (i = 0; i < PAGESZ; i++)
    buff[i] = (i * 2 + 4) % 256;

  if (segment_write(seg, 0, buff, PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_write] error");

  for (i = 0; i < PAGESZ; i++)
    buff[i] = 0;

  if (segment_read(seg, 0, buff, PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_read] error");

  for (i = 0; i < PAGESZ; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data read is different from the one written");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(ri09iawefk34hgh);

  TEST_LEAVE();
}
