/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/core/sched/context_02/context_02.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun may 27 21:26:09 2007]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int executed1 = 0;
static volatile int executed2 = 0;
static volatile int invalid = 0;
static volatile int regs[6];

static void thread1(void);

asm ("thread1:				\n"
     "1:				\n"
     "	cli				\n"
     "	movl $0x5a5a5a5a, %eax		\n"
     "	movl $0xa5a5a5a5, %ebx		\n"
     "	movl $0xffffffff, %ecx		\n"
     "	movl $0xaaaaaaaa, %edx		\n"
     "	movl $0x01010101, %esi		\n"
     "	movl $0x5a5a5a5a, %edi		\n"
     "	pushl %eax			\n"
     "	movl $regs, %eax		\n"
     "	movl %eax, 0(%eax)		\n"
     "	movl %ebx, 4(%eax)		\n"
     "	movl %ecx, 8(%eax)		\n"
     "	movl %edx, 12(%eax)		\n"
     "	movl %esi, 16(%eax)		\n"
     "	movl %edi, 20(%eax)		\n"
     "	popl %eax			\n"
     "	movl $1, executed1		\n"
     "	sti				\n"
     "	nop				\n"
     "	nop				\n"
     "	nop				\n"
     "	nop				\n"
     "	nop				\n"
     "	nop				\n"
     "	nop				\n"
     "	nop				\n"
     "	nop				\n"
     "	nop				\n"
     "	nop				\n"
     "	cli				\n"
     "	pushl %eax			\n"
     "	movl $regs, %eax		\n"
     "	cmpl %eax, 0(%eax)		\n"
     "	jne 2f				\n"
     "	cmpl %ebx, 4(%eax)		\n"
     "	jne 2f				\n"
     "	cmpl %ecx, 8(%eax)		\n"
     "	jne 2f				\n"
     "	cmpl %edx, 12(%eax)		\n"
     "	jne 2f				\n"
     "	cmpl %esi, 16(%eax)		\n"
     "	jne 2f				\n"
     "	cmpl %edi, 20(%eax)		\n"
     "	jne 2f				\n"
     "	popl %eax			\n"
     "	sti				\n"
     "	jmp 1b				\n"
     "2:				\n"
     "	movl $1, invalid		\n"
     "	sti				\n"
     "3:				\n"
     "	jmp 3b				");

static void thread2(void);

asm ("thread2:				\n"
     "1:				\n"
     "	cli				\n"
     "	movl $0x12345678, %eax		\n"
     "	movl $0xff4556a8, %ebx		\n"
     "	movl $0x01010101, %ecx		\n"
     "	movl $0x56ab78fb, %edx		\n"
     "	movl $0xabcdef, %esi		\n"
     "	movl $0x1234abcd, %edi		\n"
     "	movl $1, executed2		\n"
     "	sti				\n"
     "	jmp 1b				");

/*
 * XXX
 */

void		check_sched_context_02_entry(void)
{
  t_id		id;
  t_uint32	start;

  TEST_ENTER();

  regs[0] = regs[1] = regs[2] = regs[3] = regs[4] =regs[5] = 0 ;

  CLI();
  assert(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread1, &id) == 0,
	 "error creating thread\n");

  assert(thread_state(id, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");

  assert(check_thread_create(ktask, THREAD_PRIOR, (t_vaddr)thread2, &id) == 0,
	 "error creating thread\n");

  assert(thread_state(id, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start thread\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec())
    ;

  assert(executed1 == 1 && executed2 == 1, "Thread not executed\n");

  assert(invalid == 0, "Error in context switching, "
	"some registers are not preserved\n");

  TEST_LEAVE();
}

CHECK_WITH_THREAD(check_sched_context_02)
