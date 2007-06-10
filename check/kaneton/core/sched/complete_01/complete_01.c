/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:41:04 2006]
 */

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int executed = 0;
static volatile int* ptr;

static void	thread1(void)
{
  executed = 1;

  *ptr = 0x5a5a5a5a;

  while (1)
    ;
}

/*
 * XXX
 */

void		check_sched_complete_01(void)
{
  i_task	tsk;
  t_id		id;
  t_uint32	start;
  o_task*	o;

  TEST_ENTER();

  ASSERT(check_task_create(TASK_CLASS_PROGRAM, &tsk) == 0,
	"error creating task\n");

  ASSERT(task_get(tsk, &o) == ERROR_NONE,
	 "error in task_get\n");

  ASSERT(map_reserve(o->asid,
		     MAP_OPT_NONE,
		     PAGESZ,
		     PERM_READ | PERM_WRITE,
		     (t_vaddr*)&ptr) == ERROR_NONE,
	 "error in map_reserve\n");

  ASSERT(check_thread_create(tsk, THREAD_PRIOR, (t_vaddr)thread1, &id) == 0,
	 "error creating thread\n");

  CLI();
  ASSERT(task_state(tsk, SCHED_STATE_RUN) == ERROR_NONE,
	 "cannot start task\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && !executed)
    ;

  ASSERT(executed == 1, "Thread not executed\n")

  TEST_LEAVE();
}
