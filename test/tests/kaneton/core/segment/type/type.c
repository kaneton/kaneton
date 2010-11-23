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

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_type(void)
{
  i_segment		seg;
  o_segment*		o;

  TEST_ENTER();

  if (segment_reserve(kasid,
		      2 * PAGESZ,
		      PERM_READ | PERM_EXEC,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_perms(seg, PERM_READ | PERM_WRITE) != ERROR_NONE)
    TEST_ERROR("[segment_perms] error\n");

  if (segment_type(seg, SEGMENT_TYPE_CATCH) != ERROR_NONE)
    TEST_ERROR("[segment_type] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (o->perms != (PERM_READ | PERM_WRITE))
    TEST_ERROR("invalid segment's permissions\n");

  if (o->type != SEGMENT_TYPE_CATCH)
    TEST_ERROR("invalid segment's type\n");

  if (segment_perms(seg, ~(PERM_READ | PERM_WRITE | PERM_EXEC)) == ERROR_NONE)
    TEST_ERROR("[segment_perms] error: setting invalid permissions\n");

  if (segment_type(seg, (1 << 4)) == ERROR_NONE)
    TEST_ERROR("[segment_type] error: setting invalid type\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
