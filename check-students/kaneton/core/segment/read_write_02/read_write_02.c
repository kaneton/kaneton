#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_read_write_02(void)
{
  i_task	task;
  t_asid	as;
  t_segid	seg;
  t_uint8	buff[2 * PAGESZ];


  TEST_ENTER();
  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(as, PAGESZ, PERM_READ | PERM_WRITE, &seg) == ERROR_NONE,
	 "error reserving segment\n");

  ASSERT(segment_read(seg, 2 * PAGESZ, buff, sizeof (t_uint32)) != ERROR_NONE,
	 "error: out of bound reading\n");

  ASSERT(segment_read(seg, 0, buff, 2 * PAGESZ) != ERROR_NONE,
	 "error: size overflow reading\n");

  ASSERT(segment_write(seg, 2 * PAGESZ, buff, sizeof (t_uint32)) != ERROR_NONE,
	 "error: out of bound writing\n");

  ASSERT(segment_write(seg, 0, buff, 2 * PAGESZ) != ERROR_NONE,
	 "error: size overflow writing\n");

  TEST_LEAVE_AS(task, as);
  TEST_LEAVE();
}
