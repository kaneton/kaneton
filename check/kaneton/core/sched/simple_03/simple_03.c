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

static void thread1(void);

asm ("thread1:				\n"
     "1:				\n"
     "	cli				\n"
     "	addl $1, executed		\n"
     "	sti				\n"
     "	jmp 1b				");

/*
 * XXX
 */

void		check_sched_simple_03_entry(void)
{
  t_id		id;
  t_uint32	start;

  TEST_ENTER();

  assert(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread1, &id) == 0,
	 "error creating thread\n");

  assert(thread_state(id, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  assert(executed > 15, "Thread not executed\n")

  TEST_LEAVE();
}

CHECK_WITH_THREAD(check_sched_simple_03)
