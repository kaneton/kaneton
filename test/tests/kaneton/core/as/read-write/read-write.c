/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../kaneton/core/as/read-write/read-write.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [fri jan 14 22:56:21 2011]
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
  t_uint8		buff[4 * ___kaneton$pagesz];

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task, &as) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (segment_reserve(as,
		      2 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg1) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &useless) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as,
		      4 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg2) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &useless) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as,
		      2 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg3) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as,
		     seg1,
		     ___kaneton$pagesz,
		     REGION_OPTION_FORCE,
		     0x20000000,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(as,
		     seg2,
		     ___kaneton$pagesz,
		     REGION_OPTION_FORCE,
		     0x20001000,
		     2 * ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(as,
		     seg3,
		     0,
		     REGION_OPTION_FORCE,
		     0x20003000,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  for (i = 10; i < 4 * ___kaneton$pagesz - 12; i++)
    buff[i] = (i * 2 + 4) % 256;

  if (as_write(as,
	       buff + 10,
	       4 * ___kaneton$pagesz - 12,
	       0x2000000a) != STATUS_OK)
    TEST_ERROR("[as_write] error");

  for (i = 10; i < 4 * ___kaneton$pagesz - 12; i++)
    buff[i] = 0;

  if (segment_read(seg1,
		   ___kaneton$pagesz,
		   buff,
		   ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[segment_read] error");

  if (segment_read(seg2,
		   ___kaneton$pagesz,
		   buff + ___kaneton$pagesz,
		   2 * ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[segment_read] error");

  if (segment_read(seg3,
		   0,
		   buff + 3 * ___kaneton$pagesz,
		   ___kaneton$pagesz) != STATUS_OK)
    TEST_ERROR("[segment_read] error");

  for (i = 10; i < 4 * ___kaneton$pagesz - 12; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data read differs from the one written");

  for (i = 10; i < 4 * ___kaneton$pagesz - 12; i++)
    buff[i] = 0;

  if (as_read(as,
	      0x2000000a,
	      4 * ___kaneton$pagesz - 12,
	      buff + 10) != STATUS_OK)
    TEST_ERROR("[as_read] error");

  for (i = 10; i < 4 * ___kaneton$pagesz - 12; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data read differs from the one written");

  TEST_SIGNATURE(03ri290saoigfeio0032);

  TEST_LEAVE();
}
