/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/segment/clone/clone.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 14:21:22 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "clone.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as		kasid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_clone(void)
{
  i_segment		seg;
  i_segment		seg2;
  t_uint32		i;
  o_segment*		o;
  t_uint8		buff[PAGESZ];

  TEST_ENTER();

  if (segment_reserve(kasid,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 2 + 4) % 256;
    }

  if (segment_write(seg, 0, buff, PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_write] error\n");

  if (segment_clone(kasid, seg, &seg2) != ERROR_NONE)
    TEST_ERROR("[segment_clone] error\n");

  if (segment_get(seg2, &o) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (o->asid != kasid)
    TEST_ERROR("invalid segment's address space identfiier after cloning\n");

  if (o->perms != (PERM_READ | PERM_WRITE))
    TEST_ERROR("invalid segment's permissions after cloning\n");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid segment's size after cloning\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  if (segment_read(seg2, 0, buff, PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_read] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      if (buff[i] != (i * 2 + 4) % 256)
	TEST_ERROR("invalid data after cloning\n");
    }

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg2) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
