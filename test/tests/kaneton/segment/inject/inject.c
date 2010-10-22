/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/segment/inject/inject.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed oct 20 15:05:18 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "inject.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_inject(void)
{
  i_task		task;
  o_segment*		o;
  o_segment*		new_seg;
  i_as			as;
  i_segment		seg;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    printf("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    printf("[as_reserve] error\n");

  new_seg = malloc(sizeof (o_segment));
  new_seg->address = 0x50000000;
  new_seg->perms = PERM_READ | PERM_EXEC;
  new_seg->size = 2 * PAGESZ;
  new_seg->type = SEGMENT_TYPE_MEMORY;

  if (segment_inject(as, new_seg, &seg) != ERROR_NONE)
    printf("[segment_inject] error\n");

  if ((t_paddr)seg != 0x50000000)
    printf("invalid segment's identifier after inject\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    printf("[segment_get] error\n");

  if (o->segid != seg)
    printf("invalid segment's identifier after injection\n");

  if (o->asid != as)
    printf("invalid segment's address space identifier after injection\n");

  if (o->address != (t_uint32)seg)
    printf("invalid segment's address after injection\n");

  if (o->size != 2 * PAGESZ)
    printf("invalid segment's size after injection\n");

  if (o->perms != (PERM_READ | PERM_EXEC))
    printf("invalid segment's permissions after injection\n");

  if (segment_release(seg) != ERROR_NONE)
    printf("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    printf("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    printf("[task_release] error\n");

  TEST_LEAVE();
}
