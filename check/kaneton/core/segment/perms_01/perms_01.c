#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_perms_01(void)
{
  i_task	task;
  o_segment*	o;
  i_as		as;
  i_segment	seg;


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  assert(segment_reserve(as, PAGESZ, PERM_READ | PERM_EXEC, &seg) == ERROR_NONE,
         "Error reserving segment\n");
  assert(segment_get(seg, &o) == ERROR_NONE, "Error getting segment descriptor structure\n");
  assert(o->perms == (PERM_READ | PERM_EXEC), "Bad permission field after initialization\n");

  assert(segment_perms(seg, PERM_READ | PERM_WRITE) == ERROR_NONE,
	 "Error changing segment perms\n");
  assert(o->perms == (PERM_READ | PERM_WRITE), "Bad permission field after perms modification\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
