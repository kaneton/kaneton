/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/core/segment/permissions/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu nov 18 16:27:17 2010]
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

  if (segment_reserve(_kernel->as, PAGESZ, PERMISSION_READ | PERMISSION_WRITE, &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error\n");

  if (o->permissions != (PERMISSION_READ | PERMISSION_WRITE))
    TEST_ERROR("invalid segment's permissions\n");

  if (segment_permissions(seg, ~(PERMISSION_READ | PERMISSION_WRITE | PERMISSION_EXEC)) == ERROR_OK)
    TEST_ERROR("[segment_permissions] error: accepting invalid permissions\n");

  if (o->permissions != (PERMISSION_READ | PERMISSION_WRITE))
    TEST_ERROR("invalid segment's permissions\n");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
