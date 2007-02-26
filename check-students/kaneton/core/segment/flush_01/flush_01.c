#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_flush_01(void)
{
  i_task	task;
  o_as*		o;
  t_asid	as;
  t_segid	seg;
  t_uint32	i;
  t_setsz	before_sz;
  t_setsz	after_sz;


  TEST_ENTER();

  TEST_NEW_AS(task, as);

  ASSERT(as_get(as, &o) == ERROR_NONE, "error getting as\n");

  ASSERT(set_size(o->segments, &before_sz) == ERROR_NONE,
      "error getting AS total segments size\n");

  for (i = 0; i < 64; i++)
    RESERVE_AND_CHECK(as, 2 * (i + 1),  &seg);

  ASSERT(segment_flush(as) == ERROR_NONE, "error flushing segments\n");

  ASSERT(set_size(o->segments, &after_sz) == ERROR_NONE,
      "error getting the remaining AS total segments size\n");

  ASSERT(after_sz == 0,
      "remaining AS total segment size after different than zero\n");

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
