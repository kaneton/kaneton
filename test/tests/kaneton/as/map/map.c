/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/test/tests/kaneton/as/map/map.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun nov  7 17:13:18 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "map.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_as_map(void)
{
  i_task		task;
  i_as			as;
  int			i;
  t_vaddr		addr;
  t_setsz		sz1_before;
  t_setsz		sz1_after;
  t_setsz		sz2_before;
  t_setsz		sz2_after;
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

  if (set_size(o->segments, &sz1_before) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  if (set_size(o->regions, &sz2_before) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  for (i = 0; i < 10; i++)
    {
      if (map_reserve(as,
		      MAP_OPT_NONE,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &addr) != ERROR_NONE)
	TEST_ERROR("[map_reserve] error\n");
    }

  if (set_size(o->segments, &sz1_after) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  if (set_size(o->regions, &sz2_after) != ERROR_NONE)
    TEST_ERROR("[set_size] error\n");

  if (sz1_after < (sz1_before + 10))
    TEST_ERROR("the segments have not been reserved properly\n");

  if (sz2_after < (sz2_before + 10))
    TEST_ERROR("the regions have not been reserved properly\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
