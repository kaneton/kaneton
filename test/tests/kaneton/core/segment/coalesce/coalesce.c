/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/core/segment/coalesce/coalesce.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:09:34 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "coalesce.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_coalesce(void)
{
  i_segment		seg;
  i_segment		seg2;
  t_uint32		try;
  o_segment*		o;

  TEST_ENTER();

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

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

  if (segment_coalesce(seg, seg2, &seg) != STATUS_OK)
    TEST_ERROR("[segment_coalesce] error");

  if (segment_get(seg, &o) != STATUS_OK)
    TEST_ERROR("[segment_get] error");

  if (o->id != seg)
    TEST_ERROR("invalid segment's identifier after coalesce");

  if (o->as != _kernel.as)
    TEST_ERROR("invalid segment's address space identifier after coalesce");

  if (o->options != SEGMENT_OPTION_NONE)
    TEST_ERROR("invalid segment's options after coalesce");

  if (o->size != 2 * ___kaneton$pagesz)
    TEST_ERROR("invalid segment's size after coalesce");

  if (o->permissions != PERMISSION_READ)
    TEST_ERROR("invalid segment's permissions after coalesce");

  if (segment_get(seg2, &o) == STATUS_OK)
    TEST_ERROR("[segment_get] error: the second coalesced segment "
	       "has not been removed\n");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ,
		      SEGMENT_OPTION_NONE,
		      &seg) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

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

  if (segment_permissions(seg, PERMISSION_WRITE) != STATUS_OK)
    TEST_ERROR("[segment_permissions] error: write permission");

  if (segment_coalesce(seg, seg2, &seg) == STATUS_OK)
    TEST_ERROR("[segment_coalesce] error: segment with different "
	       "permissions\n");

  if (segment_release(seg) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  if (segment_release(seg2) != STATUS_OK)
    TEST_ERROR("[segment_release] error");

  TEST_SIGNATURE(043t90iesroikf29hg34h);

  TEST_LEAVE();
}
