/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/segment/reserve/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:07:35 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_reserve_03(void)
{
  i_segment		seg;
  o_segment*		o;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      42 * ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_get(seg, &o) != STATUS_OK)
    TEST_ERROR("[segment_get] error");

  if (o->id != seg)
    TEST_ERROR("invalid segment's identifier");

  if (o->as != _kernel.as)
    TEST_ERROR("invalid segment's address space identifier");

  if (o->size != (42 * ___kaneton$pagesz))
    TEST_ERROR("invalid segment's size");

  if (o->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(fg90i3i2fwoai43hg);

  TEST_LEAVE();
}
