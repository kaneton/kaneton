#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_inject_01(void)
{
  i_task	task;
  o_segment*	o;
  o_segment*	new_seg;
  i_as		as;
  i_segment	seg;


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  new_seg = malloc(sizeof (o_segment));
  new_seg->address = 0x50000000;
  new_seg->perms = PERM_READ | PERM_EXEC;
  new_seg->size = 2 * PAGESZ;
  new_seg->type = SEGMENT_TYPE_MEMORY;

  assert(segment_inject(as, new_seg, &seg) == ERROR_NONE,
	    "error injecting segment\n");

  assert((t_paddr)seg == 0x50000000, "Bad segment id after segment_inject\n");

  assert(segment_get(seg, &o) == ERROR_NONE, "error getting as after segment_inject\n");

  assert(o->segid == seg, "Bad segid field after segment_inject\n");
  assert(o->asid == as, "Bad asid field after segment_inject\n");
  assert(o->address == (t_uint32)seg, "Bad address field after segment_inject\n");
  assert(o->size == 2 * PAGESZ, "Bad size field after segment_inject\n");
  assert(o->perms == (PERM_READ | PERM_EXEC), "Bad perms field after segment_inject\n");

  segment_release(seg);

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
