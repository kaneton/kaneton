/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/core/segment/permissions/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri jan 14 23:00:31 2011]
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

void			test_core_segment_permissions_01(void)
{
  o_segment*		o;
  i_segment		seg;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_EXEC,
		      SEGMENT_OPTION_NONE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error");

  if (o->permissions != (PERMISSION_READ | PERMISSION_EXEC))
    TEST_ERROR("invalid segment's permissions");

  if (segment_permissions(seg, PERMISSION_READ | PERMISSION_WRITE) != ERROR_OK)
    TEST_ERROR("[segment_permissions] error");

  if (o->permissions != (PERMISSION_READ | PERMISSION_WRITE))
    TEST_ERROR("invalid segment's permissions");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(i9awefg94igy3h43h);

  TEST_LEAVE();
}
