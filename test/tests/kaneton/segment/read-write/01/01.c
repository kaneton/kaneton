/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/segment/read-write/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov  6 17:19:42 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_segment_readwrite_01(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  t_uint32		i;
  t_uint8		buff[PAGESZ];

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST, TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE, &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  for (i = 0; i < PAGESZ; i++)
    buff[i] = (i * 2 + 4) % 256;

  if (segment_write(seg, 0, buff, PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_write] error\n");

  for (i = 0; i < PAGESZ; i++)
    buff[i] = 0;

  if (segment_read(seg, 0, buff, PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_read] error\n");

  for (i = 0; i < PAGESZ; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data read is different from the one written\n");

  if (segment_release(seg) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
