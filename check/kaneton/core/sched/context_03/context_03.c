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
static volatile unsigned short cs;
static volatile unsigned short ds;
static volatile unsigned short es;
static volatile unsigned short fs;
static volatile unsigned short gs;
static volatile unsigned short ss;

static void thread1(void);

asm ("thread1:				\n"
     "	cli				\n"
     "	pushl %eax			\n"
     "	movw %cs, %ax			\n"
     "	movw %ax, cs			\n"
     "	movw %ds, %ax			\n"
     "	movw %ax, ds			\n"
     "	movw %es, %ax			\n"
     "	movw %ax, es			\n"
     "	movw %fs, %ax			\n"
     "	movw %ax, fs			\n"
     "	movw %gs, %ax			\n"
     "	movw %ax, gs			\n"
     "	movw %ss, %ax			\n"
     "	movw %ax, ss			\n"
     "	popl %eax			\n"
     "	movl $1, executed		\n"
     "	sti				\n"
     "1:				\n"
     "	jmp 1b				");

/*
 * XXX
 */

void		check_sched_context_03(void)
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

  ASSERT(cs == 0x8, "Bad value for cs\n");
  ASSERT(ds == 0x10, "Bad value for ds\n");
  ASSERT(es == 0x10, "Bad value for es\n");
  ASSERT(fs == 0x10, "Bad value for fs\n");
  ASSERT(gs == 0x10, "Bad value for gs\n");
  ASSERT(ss == 0x10, "Bad value for ss\n");

  TEST_LEAVE();
}
