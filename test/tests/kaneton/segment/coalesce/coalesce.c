/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ests/kaneton/segment/coalesce/coalesce.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov  6 22:41:47 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "coalesce.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_coalesce(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_segment		seg2;
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
		      PAGESZ,
		      PERM_READ,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

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

  if (segment_coalesce(seg, seg2, &seg) != ERROR_NONE)
    TEST_ERROR("[segment_coalesce] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    TEST_ERROR("[segment_get] error\n");

  if (o->segid != seg)
    TEST_ERROR("invalid segment's identifier after coalesce\n");

  if (o->asid != as)
    TEST_ERROR("invalid segment's address space identifier after coalesce\n");

  if (o->type != SEGMENT_TYPE_MEMORY)
    TEST_ERROR("invalid segment's type after coalesce\n");

  if (o->size != 2 * PAGESZ)
    TEST_ERROR("invalid segment's size after coalesce\n");

  if (o->perms != PERM_READ)
    TEST_ERROR("invalid segment's perms after coalesce\n");

  if (segment_get(seg2, &o) == ERROR_NONE)
    TEST_ERROR("[segment_get] error: the second coalesced segment "
	       "has not been removed\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (segment_reserve(as,
		      PAGESZ,
		      PERM_READ,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

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

  if (segment_perms(seg, PERM_WRITE) != ERROR_NONE)
    TEST_ERROR("[segment_perms] error: write permission\n");

  if (segment_coalesce(seg, seg2, &seg) == ERROR_NONE)
    TEST_ERROR("[segment_coalesce] error: segment with different "
	       "permissions\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (segment_release(seg2) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("failed to release task\n");

  TEST_LEAVE();
}
