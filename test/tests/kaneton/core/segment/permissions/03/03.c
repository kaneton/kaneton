/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...kaneton/core/segment/permissions/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:56:36 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_permissions_03(void)
{
  o_segment*		o;
  i_segment		seg;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error");

  if (o->permissions != (PERMISSION_READ | PERMISSION_WRITE))
    TEST_ERROR("invalid segment's permissions");

  if (segment_permissions(seg,
			  ~(PERMISSION_READ |
			    PERMISSION_WRITE |
			    PERMISSION_EXEC)) == ERROR_OK)
    TEST_ERROR("[segment_permissions] error: accepting invalid permissions");

  if (o->permissions != (PERMISSION_READ | PERMISSION_WRITE))
    TEST_ERROR("invalid segment's permissions");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(9r23awofk4gk3y0gh);

  TEST_LEAVE();
}
