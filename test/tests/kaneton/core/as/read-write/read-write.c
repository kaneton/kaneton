/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo.../kaneton/core/as/read-write/read-write.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov 29 18:42:21 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "read-write.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_readwrite(void)
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
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != ERROR_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != ERROR_OK)
    TEST_ERROR("[as_reserve] error");

  if (segment_reserve(as,
		      2 * PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg1) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &useless) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as,
		      4 * PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg2) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &useless) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as,
		      2 * PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg3) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
		     seg1,
		     PAGESZ,
		     REGION_OPTION_FORCE,
		     0x20000000,
		     PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(as,
		     seg2,
		     PAGESZ,
		     REGION_OPTION_FORCE,
		     0x20001000,
		     2 * PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(as,
		     seg3,
		     0,
		     REGION_OPTION_FORCE,
		     0x20003000,
		     PAGESZ,
		     &reg) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    buff[i] = (i * 2 + 4) % 256;

  if (as_write(as,
	       buff + 10,
	       4 * PAGESZ - 12,
	       0x2000000a) != ERROR_OK)
    TEST_ERROR("[as_write] error");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    buff[i] = 0;

  if (segment_read(seg1,
		   PAGESZ,
		   buff,
		   PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_read] error");

  if (segment_read(seg2,
		   PAGESZ,
		   buff + PAGESZ,
		   2 * PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_read] error");

  if (segment_read(seg3,
		   0,
		   buff + 3 * PAGESZ,
		   PAGESZ) != ERROR_OK)
    TEST_ERROR("[segment_read] error");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data read differs from the one written");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    buff[i] = 0;

  if (as_read(as,
	      0x2000000a,
	      4 * PAGESZ - 12,
	      buff + 10) != ERROR_OK)
    TEST_ERROR("[as_read] error");

  for (i = 10; i < 4 * PAGESZ - 12; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data read differs from the one written");

  TEST_SIGNATURE(03ri290saoigfeio0032);

  TEST_LEAVE();
}
