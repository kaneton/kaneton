/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/core/segment/type/type.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu nov 18 16:33:31 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "type.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_type(void)
{
  i_segment		seg;
  o_segment*		o;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      2 * PAGESZ,
		      PERMISSION_READ | PERMISSION_EXEC,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_permissions(seg, PERMISSION_READ | PERMISSION_WRITE) != ERROR_OK)
    TEST_ERROR("[segment_permissions] error\n");

  if (segment_type(seg, SEGMENT_TYPE_CATCH) != ERROR_OK)
    TEST_ERROR("[segment_type] error\n");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error\n");

  if (o->permissions != (PERMISSION_READ | PERMISSION_WRITE))
    TEST_ERROR("invalid segment's permissions\n");

  if (o->type != SEGMENT_TYPE_CATCH)
    TEST_ERROR("invalid segment's type\n");

  if (segment_permissions(seg, ~(PERMISSION_READ | PERMISSION_WRITE | PERMISSION_EXEC)) == ERROR_OK)
    TEST_ERROR("[segment_permissions] error: setting invalid permissions\n");

  if (segment_type(seg, (1 << 4)) == ERROR_OK)
    TEST_ERROR("[segment_type] error: setting invalid type\n");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
