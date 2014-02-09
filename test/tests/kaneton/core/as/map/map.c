/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...BLE/test/tests/kaneton/core/as/map/map.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:41:56 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "map.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_map(void)
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
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (as_get(as, &o) != STATUS_OK)
    TEST_ERROR("[as_get] error");

  if (set_size(o->segments, &sz1_before) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  if (set_size(o->regions, &sz2_before) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  for (i = 0; i < 10; i++)
    {
      if (map_reserve(as,
		      MAP_OPTION_NONE,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &addr) != STATUS_OK)
	TEST_ERROR("[map_reserve] error");
    }

  if (set_size(o->segments, &sz1_after) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  if (set_size(o->regions, &sz2_after) != STATUS_OK)
    TEST_ERROR("[set_size] error");

  if (sz1_after < (sz1_before + 10))
    TEST_ERROR("the segments have not been reserved properly");

  if (sz2_after < (sz2_before + 10))
    TEST_ERROR("the regions have not been reserved properly");

  if (as_release(as) != STATUS_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task) != STATUS_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(9i32aaaaaigfjergj39g);

  TEST_LEAVE();
}
