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
 * updated       julien quintard   [wed nov 24 09:42:54 2010]
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
		      2 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error\n");

  if (o->id != seg)
    TEST_ERROR("invalid segment's identifier\n");

  if (o->as != _kernel->as)
    TEST_ERROR("invalid segment's address space identifier\n");

  if (o->size != (2 * PAGESZ))
    TEST_ERROR("invalid segment's size\n");

  if (o->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions\n");

  TEST_LEAVE();
}
