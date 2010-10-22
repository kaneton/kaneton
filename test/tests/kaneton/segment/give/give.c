/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...E/test/tests/kaneton/segment/give/give.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed oct 20 14:43:33 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "give.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_give(void)
{
  i_task		task1;
  i_task		task2;
  i_as			as1;
  i_as			as2;
  i_segment		seg;
  o_segment*		o;
  o_as*			oas;
  void*			chiche;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task1) != ERROR_NONE)
      printf("[task_reserve] error\n");

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task2) != ERROR_NONE)
    printf("[task_reserve] error\n");

  if (as_reserve(task1, &as1) != ERROR_NONE)
    printf("[as_reserve] error\n");

  if (as_reserve(task2, &as2) != ERROR_NONE)
    printf("[as_reserve] error\n");

  if (segment_reserve(as1,
		      2 * PAGESZ,
		      PERM_READ,
		      &seg) != ERROR_NONE)
    printf("[segment_reserve] error\n");

  if (segment_give(as2, seg) != ERROR_NONE)
    printf("[segment_give] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    printf("[segment_get] error\n");

  if (o->asid != as2)
    printf("invalid segment's address space after give\n");

  if (as_get(as1, &oas) != ERROR_NONE)
    printf("[segment_get] error\n");

  if (set_get(oas->segments, seg, &chiche) != ERROR_NONE)
    printf("[set_get] error: the segment has not been removed from "
	   "the first address space during the give process\n");

  if (as_get(as2, &oas) != ERROR_NONE)
    printf("[as_get] error\n");

  if (set_get(oas->segments, seg, &chiche) != ERROR_NONE)
    printf("[set_get] error: the segment does not seem to have been added "
	   "to destination address space during the give process\n");

  if (as_release(as1) != ERROR_NONE)
    printf("[as_release] error\n");

  if (as_release(as2) != ERROR_NONE)
    printf("[as_release] error\n");

  if (task_release(task1) != ERROR_NONE)
    printf("[task_release] error\n");

  if (task_release(task2) != ERROR_NONE)
    printf("[task_release] error\n");

  TEST_LEAVE();
}
