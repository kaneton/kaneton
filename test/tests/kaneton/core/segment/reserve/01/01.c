/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/segment/reserve/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri jan 14 22:59:09 2011]
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

void			test_core_segment_reserve_01(void)
{
  i_segment		seg;
  o_segment*		o;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      2 * ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error");

  if (o->id != seg)
    TEST_ERROR("invalid segment's identifier");

  if (o->as != _kernel->as)
    TEST_ERROR("invalid segment's address space identifier");

  if (o->size != (2 * ___kaneton$pagesz))
    TEST_ERROR("invalid segment's size");

  if (o->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions");

  TEST_SIGNATURE(f90iw3awofhg3);

  TEST_LEAVE();
}
