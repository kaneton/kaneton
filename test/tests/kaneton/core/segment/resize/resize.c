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
 * updated       julien quintard   [sun jan 30 21:07:08 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "resize.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

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

  if (segment_reserve(_kernel.as,
		      3 * ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_resize(seg,
		     ___kaneton$pagesz,
		     SEGMENT_OPTION_NONE,
		     &seg) != STATUS_OK)
    TEST_ERROR("[segment_resize] error");

  if (segment_get(seg, &o) != STATUS_OK)
    TEST_ERROR("[segment_get] error");

  if (o->id != seg)
    TEST_ERROR("invalid segment identifier after resize");

  if (o->as != _kernel.as)
    TEST_ERROR("invalid segment's address space identifier after resize");

  if (o->options != SEGMENT_OPTION_NONE)
    TEST_ERROR("invalid segment's options after resize");

  if (o->size != ___kaneton$pagesz)
    TEST_ERROR("invalid segment's size after resize");

  if (o->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions after resize");

  try = 0;
  while (try < 40)
    {
      o_segment*	o1;
      o_segment*	o2;

      if (segment_reserve(_kernel.as,
			  ___kaneton$pagesz,
			  PERMISSION_READ,
			  SEGMENT_OPTION_NONE,
			  &seg2) != STATUS_OK)
	TEST_ERROR("[segment_reserve] error");

      if (segment_get(seg, &o1) != STATUS_OK)
	TEST_ERROR("[segment_get] error");

      if (segment_get(seg2, &o2) != STATUS_OK)
	TEST_ERROR("[segment_get] error");

      if (o2->address == o1->address + ___kaneton$pagesz)
	break;

      if (segment_release(seg) != STATUS_OK)
	TEST_ERROR("[segment_release] error");

      seg = seg2;

      try++;
    }

  if (try == 40)
    TEST_ERROR("unable to allocate a segment following a previously "
	       "reserved one");

  if (segment_resize(seg,
		     10 * ___kaneton$pagesz,
		     SEGMENT_OPTION_NONE,
		     &seg3) != STATUS_OK)
    TEST_ERROR("[segment_resize] error");

  if (seg3 == seg)
    TEST_ERROR("some segments seem to be overlapping");

  if (segment_release(seg2) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  seg = seg3;

  if (segment_get(seg, &o) != STATUS_OK)
    TEST_ERROR("[segment_get] error");

  if (o->id != seg)
    TEST_ERROR("invalid segment's identifier resize");

  if (o->as != _kernel.as)
    TEST_ERROR("invalid segment's address space identifier after resize");

  if (o->options != SEGMENT_OPTION_NONE)
    TEST_ERROR("invalid segment's options after resize");

  if (o->size != 10 * ___kaneton$pagesz)
    TEST_ERROR("invalid segment's size after resize");

  if (o->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions after resize");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(ergklzdkg4g34h);

  TEST_LEAVE();
}
