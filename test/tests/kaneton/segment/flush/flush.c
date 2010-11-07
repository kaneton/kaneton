/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/segment/flush/flush.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov  6 15:28:04 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "flush.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_flush(void)
{
  i_task		task;
  o_as*			o;
  i_as			as;
  i_segment		seg;
  t_uint32		i;
  t_setsz		before_sz;
  t_setsz		after_sz;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (as_get(as, &o) != ERROR_NONE)
    TEST_ERROR("[as_get] error\n");

  if (set_size(o->segments, &before_sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  for (i = 0; i < 64; i++)
    {
      o_segment*	s;
      t_uint32		n;

      n = 2 * (i + 1);

      if (segment_reserve(as, n * PAGESZ, PERM_READ, &seg) != ERROR_NONE)
	TEST_ERROR("[segment_reserve] error\n");

      if (segment_get(seg, &s) != ERROR_NONE)
	TEST_ERROR("[segment_get] error\n");

      if (s->segid != seg)
	TEST_ERROR("invalid segment's identifier\n");

      if (s->asid != as)
	TEST_ERROR("invalid segment's address space identifier\n");

      if (s->size != n * PAGESZ)
	TEST_ERROR("invalid segment's size\n");

      if (s->perms != PERM_READ)
	TEST_ERROR("invalid segment's permissions\n");
    }

  if (segment_flush(as) != ERROR_NONE)
    TEST_ERROR("[segment_flush] error");

  if (set_size(o->segments, &after_sz) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  if (after_sz != 0)
    TEST_ERROR("segments seem to be reamining in the set after flush\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
