#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_perms_03(void)
{
  i_task	task;
  o_segment*	o;
  i_as		as;
  i_segment	seg;


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  ASSERT(segment_reserve(as, PAGESZ, PERM_READ | PERM_WRITE, &seg) == ERROR_NONE,
         "Error reserving segment\n");
  ASSERT(segment_get(seg, &o) == ERROR_NONE, "Error getting segment descriptor structure\n");
  ASSERT(o->perms == (PERM_READ | PERM_WRITE), "Bad permission field after initialization\n");

  ASSERT(segment_perms(seg, ~(PERM_READ | PERM_WRITE | PERM_EXEC)) !=
	    ERROR_NONE,
	    "error setting bad perms\n");
  ASSERT(o->perms == (PERM_READ | PERM_WRITE), "Bad permission field after perms modification\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
