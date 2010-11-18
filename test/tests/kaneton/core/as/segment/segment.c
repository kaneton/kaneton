/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../tests/kaneton/core/as/segment/segment.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [thu nov 18 15:32:08 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "segment.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_as_segment(void)
{
  i_task		task;
  i_as			as;
  int			i;
  i_segment		seg;
  t_setsz		sz_before;
  t_setsz		sz_after;
  o_as*			o;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (as_get(as, &o) != ERROR_NONE)
    TEST_ERROR("[as_get] error\n");

  if (set_size(o->segments, &sz_before) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  for (i = 0; i < 10; i++)
    {
      if (segment_reserve(as,
			  PAGESZ,
			  PERM_READ | PERM_WRITE,
			  &seg) != ERROR_NONE)
	TEST_ERROR("[segment_reserve] error\n");
    }

  if (set_size(o->segments, &sz_after) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  if (sz_after < (sz_before + 10))
    TEST_ERROR("the segments have not been reserved properly\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
