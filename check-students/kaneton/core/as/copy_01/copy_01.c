#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_copy_01(void)
{
  i_task	task1;
  i_task	task2;
  i_as		as1;
  i_as		as2;
  i_segment	seg1;
  i_segment	seg2;
  i_segment	seg3;
  i_segment	seg4;
  i_segment	seg5;
  i_segment	seg6;
  i_segment	seg7;
  i_segment	useless;
  i_region	reg;
  t_uint32	i;
  static t_uint8	buff[4 * PAGESZ];


  TEST_ENTER();

  /* first as */

  TEST_NEW_AS(task1, as1);

  ASSERT(segment_reserve(as1, 2 * PAGESZ, PERM_READ | PERM_WRITE,
			 &seg1) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as1, PAGESZ, PERM_READ | PERM_WRITE,
			 &useless) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as1, 4 * PAGESZ, PERM_READ | PERM_WRITE,
			 &seg2) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as1, PAGESZ, PERM_READ | PERM_WRITE,
			 &useless) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as1, 2 * PAGESZ, PERM_READ | PERM_WRITE,
			 &seg3) == ERROR_NONE,
	 "error reserving segment\n");

  ASSERT(region_reserve(as1, seg1, PAGESZ, REGION_OPT_FORCE,
			0x20000000, PAGESZ, &reg) == ERROR_NONE,
	 "error reserving region\n");
  ASSERT(region_reserve(as1, seg2, PAGESZ, REGION_OPT_FORCE,
			0x20001000, 2 * PAGESZ, &reg) == ERROR_NONE,
	 "error reserving region\n");
  ASSERT(region_reserve(as1, seg3, 0, REGION_OPT_FORCE,
			0x20003000, PAGESZ, &reg) == ERROR_NONE,
	 "error reserving region\n");

  /* second as */

  TEST_NEW_AS(task2, as2);

  ASSERT(segment_reserve(as2, 2 * PAGESZ, PERM_READ | PERM_WRITE,
			 &seg4) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as2, PAGESZ, PERM_READ | PERM_WRITE,
			 &useless) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as2, 4 * PAGESZ, PERM_READ | PERM_WRITE,
			 &seg5) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as2, PAGESZ, PERM_READ | PERM_WRITE,
			 &useless) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as2, 2 * PAGESZ, PERM_READ | PERM_WRITE,
			 &seg6) == ERROR_NONE,
	 "error reserving segment\n");

  ASSERT(region_reserve(as2, seg4, 0, REGION_OPT_FORCE,
			0x40000000, PAGESZ, &reg) == ERROR_NONE,
	 "error reserving region\n");
  ASSERT(region_reserve(as2, seg5, 2 * PAGESZ, REGION_OPT_FORCE,
			0x40001000, PAGESZ, &reg) == ERROR_NONE,
	 "error reserving region\n");
  ASSERT(region_reserve(as2, seg6, 0, REGION_OPT_FORCE,
			0x40002000, 2 * PAGESZ, &reg) == ERROR_NONE,
	 "error reserving region\n");

  for (i = 0; i < 4 * PAGESZ; i++)
    {
      buff[i] = (i * 2 + 4) % 256;
    }

  ASSERT(as_write(as1, buff, 4 * PAGESZ, 0x20000000) == ERROR_NONE,
	 "error in as_write\n");

  for (i = 0; i < 4 * PAGESZ; i++)
    {
      buff[i] = 0;
    }

  ASSERT(as_copy(as1, 0x20000000, as2, 0x40000000, 4 * PAGESZ) == ERROR_NONE,
	 "error in as_copy\n");

  ASSERT(as_read(as2, 0x40000000, 4 * PAGESZ, buff) == ERROR_NONE,
	 "error in as_read\n");

  for (i = 0; i < 4 * PAGESZ; i++)
    {
      ASSERT(buff[i] == (i * 2 + 4) % 256, "bad data after copying\n");
    }

  TEST_LEAVE_AS(task1, as1);
  TEST_LEAVE_AS(task2, as2);

  TEST_LEAVE();
}
