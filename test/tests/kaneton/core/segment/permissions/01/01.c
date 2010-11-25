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
 * updated       julien quintard   [wed nov 24 09:41:11 2010]
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

  if (segment_reserve(_kernel->as, PAGESZ, PERMISSION_READ | PERMISSION_EXEC, &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error\n");

  if (o->permissions != (PERMISSION_READ | PERMISSION_EXEC))
    TEST_ERROR("invalid segment's permissions\n");

  if (segment_permissions(seg, PERMISSION_READ | PERMISSION_WRITE) != ERROR_OK)
    TEST_ERROR("[segment_permissions] error\n");

  if (o->permissions != (PERMISSION_READ | PERMISSION_WRITE))
    TEST_ERROR("invalid segment's permissions\n");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
