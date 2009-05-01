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

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int executed = 0;
static volatile int go = 0;

static void	thread1(void)
{
  CLI();
  executed++;
  STI();

  while (1)
    if (go && executed == 1)
      executed++;
}

/*
 * XXX
 */

void		check_task_clone_01_entry(void)
{
  i_task	tsk;
  i_task	cloned;
  t_id		id;
  t_uint32	start;

  TEST_ENTER();

  assert(check_task_create(TASK_CLASS_DRIVER, &tsk) == 0,
	"error creating task\n");

  assert(check_thread_create(tsk, THREAD_PRIOR, (t_vaddr)thread1, &id) == 0,
	 "error creating thread\n");

  CLI();
  assert(task_state(tsk, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start task\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && !executed)
    ;

  assert(executed == 1, "Thread not executed\n");

  go = 1;

  CLI();
  assert(task_clone(tsk, &cloned) == ERROR_NONE,
	 "cannot clone task\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && executed != 2)
    ;

  assert(executed == 2, "Cloned thread not executed\n")

  TEST_LEAVE();
}

CHECK_WITH_THREAD(check_task_clone_01)
