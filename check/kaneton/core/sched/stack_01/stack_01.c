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

static volatile t_vaddr esp1 = 0;
static volatile t_vaddr esp2 = 0;

static void thread1(void);

asm ("thread1:				\n"
     "	cli				\n"
     "	movl %esp, esp1			\n"
     "	sti				\n"
     "1:				\n"
     "	jmp 1b				");

static void thread2(void);

asm ("thread2:				\n"
     "	cli				\n"
     "	movl %esp, esp2			\n"
     "	sti				\n"
     "1:				\n"
     "	jmp 1b				");

/*
 * XXX
 */

void		check_sched_stack_01_entry(void)
{
  t_id		id1, id2;
  t_uint32	start;
  t_thread_context	ctx1, ctx2;

  TEST_ENTER();

  assert(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread1, &id1) == 0,
	 "error creating thread\n");
  assert(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread2, &id2) == 0,
	 "error creating thread\n");

  assert(thread_store(id1, &ctx1) == ERROR_NONE,
	 "error in thread_store\n");
  assert(thread_store(id2, &ctx2) == ERROR_NONE,
	 "error in thread_store\n");

  CLI();
  assert(thread_state(id1, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");
  assert(thread_state(id2, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && (!esp1 || !esp2))
    ;

  assert(esp1 != 0 && esp2 != 0, "Thread not executed\n");

  assert(esp1 - 2048 < ctx1.sp && esp1 + 2048 > ctx1.sp, "Bad stack\n");
  assert(esp2 - 2048 < ctx2.sp && esp2 + 2048 > ctx2.sp, "Bad stack\n");

  TEST_LEAVE();
}

CHECK_WITH_THREAD(check_sched_stack_01)
