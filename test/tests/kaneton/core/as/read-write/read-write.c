/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...tests/kaneton/as/read-write/read-write.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [wed nov 17 07:58:12 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "read-write.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_as_readwrite(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg1;
  i_segment		seg2;
  i_segment		seg3;
  i_segment		useless;
  i_region		reg;
  t_uint32		i;
  t_uint8		buff[4 * PAGESZ];

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      2 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg1) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &useless) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as,
		      4 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg2) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &useless) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (segment_reserve(as,
		      2 * PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &seg3) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(as,
		     seg1,
		     PAGESZ,
		     REGION_OPT_FORCE,
		     0x20000000,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_reserve(as,
		     seg2,
		     PAGESZ,
		     REGION_OPT_FORCE,
		     0x20001000,
		     2 * PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  if (region_reserve(as,
		     seg3,
		     0,
		     REGION_OPT_FORCE,
		     0x20003000,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    buff[i] = (i * 2 + 4) % 256;

  if (as_write(as,
	       buff + 10,
	       4 * PAGESZ - 12,
	       0x2000000a) != ERROR_NONE)
    TEST_ERROR("[as_write] error\n");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    buff[i] = 0;

  if (segment_read(seg1,
		   PAGESZ,
		   buff,
		   PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_read] error\n");

  if (segment_read(seg2,
		   PAGESZ,
		   buff + PAGESZ,
		   2 * PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_read] error\n");

  if (segment_read(seg3,
		   0,
		   buff + 3 * PAGESZ,
		   PAGESZ) != ERROR_NONE)
    TEST_ERROR("[segment_read] error\n");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data read differs from the one written\n");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    buff[i] = 0;

  if (as_read(as,
	      0x2000000a,
	      4 * PAGESZ - 12,
	      buff + 10) != ERROR_NONE)
    TEST_ERROR("[as_read] error\n");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data read differs from the one written\n");

  TEST_LEAVE();
}
