/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/segment/resize/resize.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 24 09:44:22 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "resize.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*	_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_resize(void)
{
  i_segment		seg;
  i_segment		seg2;
  i_segment		seg3;
  t_uint32		try;
  o_segment*		o;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      3 * PAGESZ,
		      PERMISSION_READ,
		      &seg) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_resize(seg, PAGESZ, &seg) != ERROR_OK)
    TEST_ERROR("[segment_resize] error\n");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error\n");

  if (o->id != seg)
    TEST_ERROR("invalid segment identifier after resize\n");

  if (o->as != _kernel->as)
    TEST_ERROR("invalid segment's address space identifier after resize\n");

  if (o->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type after resize\n");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid segment's size after resize\n");

  if (o->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions after resize\n");

  try = 0;
  while (try < 40)
    {
      o_segment*	o1;
      o_segment*	o2;

      if (segment_reserve(_kernel->as,
			  PAGESZ,
			  PERMISSION_READ,
			  &seg2) != ERROR_OK)
	TEST_ERROR("[segment_reserve] error\n");

      if (segment_get(seg, &o1) != ERROR_OK)
	TEST_ERROR("[segment_get] error\n");

      if (segment_get(seg2, &o2) != ERROR_OK)
	TEST_ERROR("[segment_get] error\n");

      if (o2->address == o1->address + PAGESZ)
	break;

      if (segment_release(seg) != ERROR_OK)
	TEST_ERROR("[segment_release] error\n");

      seg = seg2;

      try++;
    }

  if (try == 40)
    TEST_ERROR("unable to allocate a segment following a previously "
	       "reserved one");

  if (segment_resize(seg, 10 * PAGESZ, &seg3) != ERROR_OK)
    TEST_ERROR("[segment_resize] error\n");

  if (seg3 == seg)
    TEST_ERROR("some segments seem to be overlapping\n");

  if (segment_release(seg2) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  seg = seg3;

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error\n");

  if (o->id != seg)
    TEST_ERROR("invalid segment's identifier resize\n");

  if (o->as != _kernel->as)
    TEST_ERROR("invalid segment's address space identifier after resize\n");

  if (o->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type after resize\n");

  if (o->size != 10 * PAGESZ)
    TEST_ERROR("invalid segment's size after resize\n");

  if (o->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions after resize\n");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  TEST_LEAVE();
}
