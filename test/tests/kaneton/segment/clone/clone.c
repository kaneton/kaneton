/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...test/tests/kaneton/segment/clone/clone.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed oct 20 12:42:14 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "clone.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_clone(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  t_uint32		i;
  o_segment*		o;
  static t_uint8	buff[PAGESZ];

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    printf("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    printf("[as_reserve] error\n");

  if (segment_reserve(as,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    printf("[segment_reserve] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 2 + 4) % 256;
    }

  if (segment_write(seg, 0, buff, PAGESZ) != ERROR_NONE)
    printf("[segment_write] error\n");

  if (segment_clone(as, seg, &seg) != ERROR_NONE)
    printf("[segment_clone] error\n");

  if (segment_get(seg, &o) != ERROR_NONE)
    printf("[segment_get] error\n");

  if (o->asid != as)
    printf("invalid segment's address space identfiier after cloning\n");

  if (o->segid != seg)
    printf("invalid segment's identifier after cloning\n");

  if (o->address != (t_paddr)seg)
    printf("invalid segment's address after cloning\n");

  if (o->perms != (PERM_READ | PERM_WRITE))
    printf("invalid segment's permissions after cloning\n");

  if (o->size != PAGESZ)
    printf("invalid segment's size after cloning\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  if (segment_read(seg, 0, buff, PAGESZ) != ERROR_NONE)
    printf("[segment_read] error\n");

  for (i = 0; i < PAGESZ; i++)
    {
      if (buff[i] != (i * 2 + 4) % 256)
	printf("invalid data after cloning\n");
    }

  if (as_release(as) != ERROR_NONE)
    printf("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    printf("[task_release] error\n");

  TEST_LEAVE();
}
