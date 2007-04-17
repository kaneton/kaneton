#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_as_read_write_01(void)
{
  i_task	task;
  i_as		as;
  i_segment	seg1;
  i_segment	seg2;
  i_segment	seg3;
  i_segment	useless;
  i_region	reg;
  t_uint32	i;
  static t_uint8	buff[4 * PAGESZ];


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(as, 2 * PAGESZ, PERM_READ | PERM_WRITE,
			 &seg1) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as, PAGESZ, PERM_READ | PERM_WRITE,
			 &useless) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as, 4 * PAGESZ, PERM_READ | PERM_WRITE,
			 &seg2) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as, PAGESZ, PERM_READ | PERM_WRITE,
			 &useless) == ERROR_NONE,
	 "error reserving segment\n");
  ASSERT(segment_reserve(as, 2 * PAGESZ, PERM_READ | PERM_WRITE,
			 &seg3) == ERROR_NONE,
	 "error reserving segment\n");

  ASSERT(region_reserve(as, seg1, PAGESZ, REGION_OPT_FORCE,
			0x20000000, PAGESZ, &reg) == ERROR_NONE,
	 "error reserving region\n");
  ASSERT(region_reserve(as, seg2, PAGESZ, REGION_OPT_FORCE,
			0x20001000, 2 * PAGESZ, &reg) == ERROR_NONE,
	 "error reserving region\n");
  ASSERT(region_reserve(as, seg3, 0, REGION_OPT_FORCE,
			0x20002000, PAGESZ, &reg) == ERROR_NONE,
	 "error reserving region\n");

  for (i = 0; i < 4 * PAGESZ; i++)
    {
      buff[i] = (i * 2 + 4) % 256;
    }

  ASSERT(as_write(as, buff, 4 * PAGESZ, 0x20000000) == ERROR_NONE,
	 "error in as_write\n");

  for (i = 0; i < 4 * PAGESZ; i++)
    {
      buff[i] = 0;
    }

  ASSERT(segment_read(seg1, PAGESZ, buff, PAGESZ) == ERROR_NONE,
	 "error reading\n");
  ASSERT(segment_read(seg2, PAGESZ, buff + PAGESZ, 2 * PAGESZ) == ERROR_NONE,
	 "error reading\n");
  ASSERT(segment_read(seg3, 0, buff + 3 * PAGESZ, PAGESZ) == ERROR_NONE,
	 "error reading\n");

  for (i = 0; i < 4 * PAGESZ; i++)
    {
      ASSERT(buff[i] == (i * 2 + 4) % 256, "bad data after writing\n");
    }

  for (i = 0; i < 4 * PAGESZ; i++)
    {
      buff[i] = 0;
    }

  ASSERT(as_read(as, 0x20000000, 4 * PAGESZ, buff) == ERROR_NONE,
	 "error in as_read\n");

  for (i = 0; i < 4 * PAGESZ; i++)
    {
      ASSERT(buff[i] == (i * 2 + 4) % 256, "bad data after reading\n");
    }

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
