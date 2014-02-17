/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/core/segment/clone/clone.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:09:02 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "clone.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_clone(void)
{
  i_segment		seg;
  i_segment		seg2;
  t_uint32		i;
  o_segment*		o;
  t_uint8		buff[___kaneton$pagesz];

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  for (i = 0; i < ___kaneton$pagesz; i++)
    {
      buff[i] = (i * 2 + 4) % 256;
    }

  if (segment_write(seg, 0, buff, ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[segment_write] error");

  if (segment_clone(_kernel.as, seg, &seg2) != STATUS_OK)
    TEST_ERROR("[segment_clone] error");

  if (segment_get(seg2, &o) != STATUS_OK)
    TEST_ERROR("[segment_get] error");

  if (o->as != _kernel.as)
    TEST_ERROR("invalid segment's address space identfiier after cloning");

  if (o->permissions != (PERMISSION_READ | PERMISSION_WRITE))
    TEST_ERROR("invalid segment's permissions after cloning");

  if (o->size != ___kaneton$pagesz)
    TEST_ERROR("invalid segment's size after cloning");

  for (i = 0; i < ___kaneton$pagesz; i++)
    {
      buff[i] = 0;
    }

  if (segment_read(seg2, 0, buff, ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[segment_read] error");

  for (i = 0; i < ___kaneton$pagesz; i++)
    {
      if (buff[i] != (i * 2 + 4) % 256)
	TEST_ERROR("invalid data after cloning");
    }

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg2) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(09tgi30g9a9wegoh);

  TEST_LEAVE();
}
