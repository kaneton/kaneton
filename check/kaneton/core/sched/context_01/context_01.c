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
static volatile unsigned int eflags;

static void thread1(void);

asm ("thread1:				\n"
     "	cli				\n"
     "	pushl %eax			\n"
     "	pushf				\n"
     "	popl %eax			\n"
     "	mov %eax, eflags		\n"
     "	popl %eax			\n"
     "	movl $1, executed		\n"
     "	sti				\n"
     "1:				\n"
     "	jmp 1b				");

/*
 * XXX
 */

void		check_sched_context_01(void)
{
  t_id		id;
  t_uint32	start;

  TEST_ENTER();

  ASSERT(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread1, &id) == 0,
	 "error creating thread\n");

  ASSERT(thread_state(id, SCHED_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && !executed)
    ;

  ASSERT(executed == 1, "Thread not executed\n");

  ASSERT(!(eflags & (1 << 0)), "flag CF should not be set\n");
  ASSERT(!(eflags & (1 << 2)), "flag PF should not be set\n");
  ASSERT(!(eflags & (1 << 6)), "flag ZF should not be set\n");
  ASSERT(!(eflags & (1 << 7)), "flag SF should not be set\n");
  ASSERT(!(eflags & (1 << 11)), "flag OF should not be set\n");

  TEST_LEAVE();
}
