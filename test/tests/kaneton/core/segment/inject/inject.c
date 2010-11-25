/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/core/segment/inject/inject.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 24 09:40:54 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "inject.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_segment_inject(void)
{
  i_task		task;
  i_as			as;
  o_segment*		o;
  o_segment*		new_seg;
  i_segment		seg;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE, &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error\n");

  new_seg = malloc(sizeof (o_segment));
  new_seg->address = 0x50000000;
  new_seg->permissions = PERMISSION_READ | PERMISSION_EXEC;
  new_seg->size = 2 * PAGESZ;
  new_seg->type = SEGMENT_TYPE_MEMORY;

  if (segment_inject(as, new_seg, &seg) != ERROR_OK)
    TEST_ERROR("[segment_inject] error\n");

  if (segment_get(seg, &o) != ERROR_OK)
    TEST_ERROR("[segment_get] error\n");

  if (o->id != seg)
    TEST_ERROR("invalid segment's identifier after injection\n");

  if (o->as != as)
    TEST_ERROR("invalid segment's address space identifier after injection\n");

  if (o->address != new_seg->address)
    TEST_ERROR("invalid segment's address after injection\n");

  if (o->size != 2 * PAGESZ)
    TEST_ERROR("invalid segment's size after injection\n");

  if (o->permissions != (PERMISSION_READ | PERMISSION_EXEC))
    TEST_ERROR("invalid segment's permissions after injection\n");

  if (segment_release(seg) != ERROR_OK)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_OK)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_OK)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
