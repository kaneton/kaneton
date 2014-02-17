/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/core/segment/flush/flush.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri jan 14 22:57:42 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "flush.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_flush(void)
{
  i_task		task;
  o_as*			o;
  i_as			as;
  i_segment		seg;
  t_uint32		i;
  t_setsz		before_sz;
  t_setsz		after_sz;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (as_get(as, &o) != STATUS_OK)
    TEST_ERROR("[as_get] error");

  if (set_size(o->segments, &before_sz) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  for (i = 0; i < 64; i++)
    {
      o_segment*	s;
      t_uint32		n;

      n = 2 * (i + 1);

      if (segment_reserve(as,
			  n * ___kaneton$pagesz,
			  PERMISSION_READ,
			  SEGMENT_OPTION_NONE,
			  &seg) != STATUS_OK)
	TEST_ERROR("[segment_reserve] error");

      if (segment_get(seg, &s) != STATUS_OK)
	TEST_ERROR("[segment_get] error");

      if (s->id != seg)
	TEST_ERROR("invalid segment's identifier");

      if (s->as != as)
	TEST_ERROR("invalid segment's address space identifier");

      if (s->size != n * ___kaneton$pagesz)
	TEST_ERROR("invalid segment's size");

      if (s->permissions != PERMISSION_READ)
	TEST_ERROR("invalid segment's permissions");
    }

  if (segment_flush(as) != STATUS_OK)
    TEST_ERROR("[segment_flush] error");

  if (set_size(o->segments, &after_sz) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  if (after_sz != 0)
    TEST_ERROR("segments seem to be reamining in the set after flush");

  TEST_SIGNATURE(i23932rkfargl320g940);

  TEST_LEAVE();
}
