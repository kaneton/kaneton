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

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int executed = 0;
static volatile int error = 1;
static volatile int tutu = 0;

static void thread1(void);

asm ("thread1:				\n"
     "	cli				\n"
     "	addl $1, executed		\n"
     "	sti				\n"
     "	movl $1, tutu			\n"
     "	int $56				\n"
     "	nop				\n"
     "	cmpl $1, tutu			\n"
     "	je 2f				\n"
     "	movl $0, error			\n"
     "2:				\n"
     "	cli				\n"
     "	addl $1, executed		\n"
     "	sti				\n"
     "1:				\n"
     "	jmp 1b				");

static void thread2(void);

asm ("thread2:				\n"
     "	cli				\n"
     "	addl $1, executed		\n"
     "	movl $0, tutu			\n"
     "	sti				\n"
     "1:				\n"
     "	movl $0, tutu			\n"
     "	jmp 1b				");

/*
 * XXX
 */

void		check_sched_yield_01(void)
{
  t_id		id1, id2;
  t_uint32	start;

  TEST_ENTER();

  event_release(56);
  ASSERT(event_reserve(56,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(sched_yield)) == ERROR_NONE,
	 "error in syscall reserve\n");

  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread1, &id1) == 0,
	 "error creating thread\n");
  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread2, &id2) == 0,
	 "error creating thread\n");

  CLI();
  ASSERT(thread_state(id1, SCHED_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");
  ASSERT(thread_state(id2, SCHED_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && executed != 2)
    ;

  ASSERT(executed == 3, "Thread not executed\n");
  ASSERT(error == 0, "Thread not yield correctly\n");

  TEST_LEAVE();
}