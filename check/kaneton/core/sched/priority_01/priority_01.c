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

static volatile unsigned int executed1 = 0;
static volatile unsigned int executed2 = 0;

static void thread1(void);

asm ("thread1:				\n"
     "1:				\n"
     "	cli				\n"
     "	addl $1, executed1		\n"
     "	sti				\n"
     "	jmp 1b				");

static void thread2(void);

asm ("thread2:				\n"
     "1:				\n"
     "	cli				\n"
     "	addl $1, executed2		\n"
     "	sti				\n"
     "	jmp 1b				");

/*
 * XXX
 */

void		check_sched_priority_01(void)
{
  t_id		id1, id2;
  t_uint32	start;

  TEST_ENTER();

  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread1, &id1) == 0,
	 "error creating thread\n");
  ASSERT(check_thread_create(ktask, THREAD_HPRIOR, (t_vaddr)thread2, &id2) == 0,
	 "error creating thread\n");

  CLI();
  ASSERT(thread_state(id1, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");
  ASSERT(thread_state(id2, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  ASSERT(executed1 != 0 && executed2 != 0, "Thread not executed\n");

  CLI();
  ASSERT(thread_state(id1, SCHEDULER_STATE_STOP) == ERROR_NONE,
	 "cannot start thread\n");
  ASSERT(thread_state(id2, SCHEDULER_STATE_STOP) == ERROR_NONE,
	 "cannot start thread\n");

  ASSERT(executed1 < executed2, "Priority not respected\n");

  STI();

  TEST_LEAVE();
}
