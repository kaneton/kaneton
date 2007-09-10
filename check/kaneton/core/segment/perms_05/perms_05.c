#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

extern i_as kasid;

void		check_segment_perms_05(void)
{
  i_task	task;
  i_as		as;
  i_segment	seg;
  i_segment	seg_ref;
  t_uint32	i;
  static char	buff[PAGESZ];


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(as, PAGESZ, PERM_READ | PERM_WRITE, &seg_ref) == ERROR_NONE,
         "Error reserving segment\n");

  ASSERT(segment_reserve(as,
			    PAGESZ,
			    PERM_READ,
			    &seg) == ERROR_NONE,
	    "error reserving segment\n");

  for (i = 0; i < PAGESZ; i++)
    {
      buff[i] = (i * 4 - 1) % 256;
    }

  ASSERT(segment_write(seg_ref, 0, buff, PAGESZ) == ERROR_NONE,
	 "error writing\n");

  ASSERT(segment_copy(seg, 0, seg_ref, 0,
			 PAGESZ) != ERROR_NONE,
	    "copy to ro segment\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
