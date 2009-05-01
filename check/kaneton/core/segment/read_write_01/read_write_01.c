#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_read_write_01(void)
{
  i_task	task;
  i_as		as;
  i_segment	seg;
  t_uint32	i;
  static t_uint8	buff[PAGESZ];


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 2 + 4) % 256;
    }

  assert(segment_reserve(as,
			    PAGESZ,
			    PERM_READ | PERM_WRITE,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  assert(segment_write(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error writing\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = 0;
    }

  assert(segment_read(seg, 0, buff, PAGESZ) == ERROR_NONE,
	    "error reading\n");

  for (i = 0; i < PAGESZ; i++)
    {
      assert(buff[i] == (i * 2 + 4) % 256, "bad data\n");
    }

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
