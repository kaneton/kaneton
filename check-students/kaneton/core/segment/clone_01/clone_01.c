#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

void		check_segment_clone_01(void)
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

  TEST_LEAVE_AS(task, as);

  TEST_LEAVE();
}
