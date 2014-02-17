/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../kaneton/core/segment/read-write/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:08:08 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_readwrite_02(void)
{
  i_segment		seg;
  char			buff[2 * ___kaneton$pagesz];

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_read(seg,
		   2 * ___kaneton$pagesz,
		   buff,
		   sizeof (t_uint32)) == STATUS_OK)
    TEST_ERROR("[segment_read] error: out of bound");

  if (segment_read(seg, 0, buff, 2 * ___kaneton$pagesz) == STATUS_OK)
    TEST_ERROR("[segment_read] error: overflow");

  if (segment_write(seg,
		    2 * ___kaneton$pagesz,
		    buff,
		    sizeof (t_uint32)) == STATUS_OK)
    TEST_ERROR("[segment_write] error: out of bound");

  if (segment_write(seg, 0, buff, 2 * ___kaneton$pagesz) == STATUS_OK)
    TEST_ERROR("[segment_write] error: overflow");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(0r320afkeoaweigj3g43);

  TEST_LEAVE();
}
