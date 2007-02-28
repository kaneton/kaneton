#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_perms_04(void)
{
  i_task	task;
  o_segment*	o;
  t_asid	as;
  t_segid	seg;
  t_uint32	i=0x41424344;


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(as, PAGESZ, PERM_READ , &seg) == ERROR_NONE,
         "Error reserving segment\n");
  ASSERT(segment_get(seg, &o) == ERROR_NONE, "Error getting segment descriptor structure\n");

  ASSERT(segment_write(seg, 0, &i, sizeof(t_uint32)) != ERROR_NONE,
	    "error: wrote to ro segment\n");


  ASSERT(segment_release(seg) == ERROR_NONE, "error releasing segment\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
