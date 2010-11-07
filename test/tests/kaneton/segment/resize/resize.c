/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/segment/resize/resize.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov  6 18:35:08 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "resize.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_resize(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_segment		seg2;
  i_segment		seg3;
  t_uint32		try;
  o_segment*		o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      3 * PAGESZ,
		      PERM_READ,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_resize(seg, PAGESZ, &seg) != ERROR_NONE)
    TEST_ERROR("[segment_resize] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (o->segid != seg)
    TEST_ERROR("invalid segment identifier after resize\n");

  if (o->asid != as)
    TEST_ERROR("invalid segment's address space identifier after resize\n");

  if (o->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type after resize\n");

  if (o->size != PAGESZ)
    TEST_ERROR("invalid segment's size after resize\n");

  if (o->perms != PERM_READ)
    TEST_ERROR("invalid segment's permissions after resize\n");

  try = 0;
  while (try < 40)
    {
      o_segment*	o1;
      o_segment*	o2;

      if (segment_reserve(as,
			  PAGESZ,
			  PERM_READ,
			  &seg2) != ERROR_NONE)
	TEST_ERROR("[segment_reserve] error\n");

      if (segment_get(seg, &o1) != ERROR_NONE)
	TEST_ERROR("[segment_get] error\n");

      if (segment_get(seg2, &o2) != ERROR_NONE)
	TEST_ERROR("[segment_get] error\n");

      if (o2->address == o1->address + PAGESZ)
	break;

      if (segment_release(seg) != ERROR_NONE)
	TEST_ERROR("[segment_release] error\n");

      seg = seg2;

      try++;
    }

  if (try == 40)
    TEST_ERROR("unable to allocate a segment following a previously "
	       "reserved one");

  if (segment_resize(seg, 10 * PAGESZ, &seg3) != ERROR_NONE)
    TEST_ERROR("[segment_resize] error\n");

  if (seg3 == seg)
    TEST_ERROR("some segments seem to be overlapping\n");

  if (segment_release(seg2) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  seg = seg3;

  if (segment_get(seg, &o) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (o->segid != seg)
    TEST_ERROR("invalid segment's identifier resize\n");

  if (o->asid != as)
    TEST_ERROR("invalid segment's address space identifier after resize\n");

  if (o->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type after resize\n");

  if (o->size != 10 * PAGESZ)
    TEST_ERROR("invalid segment's size after resize\n");

  if (o->perms != PERM_READ)
    TEST_ERROR("invalid segment's permissions after resize\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("failed to release task\n");

  TEST_LEAVE();
}
