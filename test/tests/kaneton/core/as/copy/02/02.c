/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/test/tests/kaneton/core/as/copy/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [fri jan 14 22:55:52 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_as_copy_02(void)
{
  i_task		task1;
  i_task		task2;
  i_as			as1;
  i_as			as2;
  i_segment		seg1;
  i_segment		seg2;
  i_segment		seg3;
  i_segment		seg4;
  i_segment		seg5;
  i_segment		seg6;
  i_segment		useless;
  i_region		reg;
  t_uint32		i;
  t_uint8		buff[4 * ___kaneton$pagesz];

  TEST_ENTER();

  /*
   * first address space
   */

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task1) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task1, &as1) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (segment_reserve(as1,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg1) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as1,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &useless) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as1,
		      2 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg2) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as1,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &useless) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as1,
		      4 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg3) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as1,
		     seg1,
		     0,
		     REGION_OPTION_FORCE,
		     0x20000000,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(as1,
		     seg2,
		     0,
		     REGION_OPTION_FORCE,
		     0x20001000,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(as1,
		     seg3,
		     ___kaneton$pagesz,
		     REGION_OPTION_FORCE,
		     0x20002000,
		     2 * ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  /*
   * second address space
   */

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAVIOUR_INTERACTIVE,
		   TASK_PRIORITY_INTERACTIVE,
		   &task2) != STATUS_OK)
    TEST_ERROR("[task_reserve] error");

  if (as_reserve(task2, &as2) != STATUS_OK)
    TEST_ERROR("[as_reserve] error");

  if (segment_reserve(as2,
		      2 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg4) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as2,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &useless) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as2,
		      4 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg5) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as2,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &useless) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (segment_reserve(as2,
		      2 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_NONE,
		      &seg6) != STATUS_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(as2,
		     seg4,
		     0,
		     REGION_OPTION_FORCE,
		     0x40000000,
		     2 * ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(as2,
		     seg5,
		     ___kaneton$pagesz,
		     REGION_OPTION_FORCE,
		     0x40002000,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  if (region_reserve(as2,
		     seg6,
		     0,
		     REGION_OPTION_FORCE,
		     0x40003000,
		     ___kaneton$pagesz,
		     &reg) != STATUS_OK)
    TEST_ERROR("[region_reserve] error");

  /*
   * operations
   */

  for (i = 0; i < 4 * ___kaneton$pagesz; i++)
    buff[i] = (i * 2 + 4) % 256;

  if (as_write(as1, buff, 4 * ___kaneton$pagesz, 0x20000000) != STATUS_OK)
    TEST_ERROR("[as_write] error");

  for (i = 0; i < 4 * ___kaneton$pagesz; i++)
    buff[i] = 0;

  if (as_copy(as1,
	      0x20000100,
	      as2,
	      0x40000100,
	      4 * ___kaneton$pagesz - 0x200) != STATUS_OK)
    TEST_ERROR("[as_copy] error");

  if (as_read(as2, 0x40000000, 4 * ___kaneton$pagesz, buff) != STATUS_OK)
    TEST_ERROR("[as_read] error");

  for (i = 0x100; i < 4 * ___kaneton$pagesz - 0x200; i++)
    if (buff[i] != (i * 2 + 4) % 256)
      TEST_ERROR("the data appears invalid once read from the "
		 "address space\n");

  if (as_release(as1) != STATUS_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task1) != STATUS_OK)
    TEST_ERROR("[task_release] error");

  if (as_release(as2) != STATUS_OK)
    TEST_ERROR("[as_release] error");

  if (task_release(task2) != STATUS_OK)
    TEST_ERROR("[task_release] error");

  TEST_SIGNATURE(032f09izoisf0w9gi0e9);

  TEST_LEAVE();
}
