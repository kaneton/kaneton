#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_inject_01(void)
{
  i_task	task;
  o_segment*	o;
  o_segment	new_seg;
  t_asid	as;
  t_segid	seg;


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  new_seg.address = 0x400000;
  new_seg.perms = PERM_READ | PERM_EXEC;
  new_seg.size = 2 * PAGESZ;
  new_seg.type = SEGMENT_TYPE_MEMORY;

  ASSERT(segment_inject(as, &new_seg, &seg) == ERROR_NONE,
	    "error injecting segment\n");

  ASSERT((t_paddr)seg == 0x400000, "Bad segment id after segment_inject\n");

  ASSERT(segment_get(seg, &o) == ERROR_NONE, "error getting as after segment_inject\n");

  ASSERT(o->segid == seg, "Bad segid field after segment_inject\n");
  ASSERT(o->asid == as, "Bad asid field after segment_inject\n");
  ASSERT(o->type == SEGMENT_TYPE_MEMORY, "Bad type field after segment_inject\n");
  ASSERT(o->address == (t_uint32)seg, "Bad address field after segment_inject\n");
  ASSERT(o->size == 2 * PAGESZ, "Bad size field after segment_inject\n");
  ASSERT(o->perms == (PERM_READ | PERM_EXEC), "Bad perms field after segment_inject\n");

  ASSERT(segment_release(seg) == ERROR_NONE, "error releasing segment after segment_inject\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
