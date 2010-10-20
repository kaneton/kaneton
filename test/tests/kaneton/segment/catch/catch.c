/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/segment/catch/catch.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon oct 18 10:12:07 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "catch.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_catch(void)
{
  i_task		task1;
  i_task		task2;
  i_as			as1;
  i_as			as2;
  i_segment		seg;
  o_segment*		o;

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

  if (segment_type(seg, SEGMENT_TYPE_CATCH) != ERROR_NONE)
    printf("[segment_type] unable to set the segment as catchable\n");

  if (segment_catch(as2, seg) != ERROR_NONE)
    printf("[segment_catch] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    printf("[segment_get] error\n");

  if (o->asid != as2)
    printf("invalid address space id after catch\n");

  if (o->type != SEGMENT_TYPE_MEMORY)
    printf("invalid type field after catch\n");

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
