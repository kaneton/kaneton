/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/scheduler/context/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [fri dec  3 22:28:29 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;
extern t_init*			_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile t_uint32	executed_01 = 0;
static volatile t_uint32	executed_02 = 0;

static volatile t_uint32	invalid = 0;

static volatile t_uint32	regs[6];

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_scheduler_context_02_thread_01 (void);
asm ("test_architecture_scheduler_context_02_thread_01:\n"
     "1:\n"
     "  cli\n"
     "  movl $0x5a5a5a5a, %eax\n"
     "  movl $0xa5a5a5a5, %ebx\n"
     "  movl $0xffffffff, %ecx\n"
     "  movl $0xaaaaaaaa, %edx\n"
     "  movl $0x01010101, %esi\n"
     "  movl $0x5a5a5a5a, %edi\n"
     "  pushl %eax\n"
     "  movl $regs, %eax\n"
     "  movl %eax, 0(%eax)\n"
     "  movl %ebx, 4(%eax)\n"
     "  movl %ecx, 8(%eax)\n"
     "  movl %edx, 12(%eax)\n"
     "  movl %esi, 16(%eax)\n"
     "  movl %edi, 20(%eax)\n"
     "  popl %eax\n"
     "  movl $1, executed_01\n"
     "  sti\n"
     "  nop\n"
     "  nop\n"
     "  nop\n"
     "  nop\n"
     "  nop\n"
     "  nop\n"
     "  nop\n"
     "  nop\n"
     "  nop\n"
     "  nop\n"
     "  nop\n"
     "  cli\n"
     "  pushl %eax\n"
     "  movl $regs, %eax\n"
     "  cmpl %eax, 0(%eax)\n"
     "  jne 2f\n"
     "  cmpl %ebx, 4(%eax)\n"
     "  jne 2f\n"
     "  cmpl %ecx, 8(%eax)\n"
     "  jne 2f\n"
     "  cmpl %edx, 12(%eax)\n"
     "  jne 2f\n"
     "  cmpl %esi, 16(%eax)\n"
     "  jne 2f\n"
     "  cmpl %edi, 20(%eax)\n"
     "  jne 2f\n"
     "  popl %eax\n"
     "  sti\n"
     "  jmp 1b\n"
     "2:\n"
     "  movl $1, invalid\n"
     "  sti\n"
     "3:\n"
     "  jmp 3b");

void			test_architecture_scheduler_context_02_thread_02 (void);
asm ("test_architecture_scheduler_context_02_thread_02:\n"
     "1:\n"
     "  cli\n"
     "  movl $0x12345678, %eax\n"
     "  movl $0xff4556a8, %ebx\n"
     "  movl $0x01010101, %ecx\n"
     "  movl $0x56ab78fb, %edx\n"
     "  movl $0xabcdef, %esi\n"
     "  movl $0x1234abcd, %edi\n"
     "  movl $1, executed_02\n"
     "  sti\n"
     "  jmp 1b");

void			test_architecture_scheduler_context_02_content(void)
{
  i_thread		thread;
  t_thread_context	ctx;
  t_stack		stack;
  o_thread*		t;
  t_clock		clock;
  t_uint64		start;

  /*
   * thread 1
   */

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_architecture_scheduler_context_02_thread_01;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_HANG("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_HANG("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_HANG("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_architecture_scheduler_context_02_thread_02;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_HANG("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  /*
   * wait
   */

  if (clock_current(&clock) != ERROR_OK)
    TEST_HANG("[clock_current] error");

  start = CLOCK_UNIQUE(&clock);

  while (1)
    {
      t_uint64		current;

      if (clock_current(&clock) != ERROR_OK)
	TEST_HANG("[clock_current] error");

      current = CLOCK_UNIQUE(&clock);

      if (current > (start + 3000))
	break;
    }

  /*
   * check
   */

  if ((executed_01 != 1) || (executed_02 != 1))
    TEST_HANG("one of the two threads have not been scheduled");

  if (invalid == 1)
    TEST_HANG("an error occured while context switching: some registers "
	      "have not been preserved");

  TEST_SIGNATURE(vmwif34wgk4eh09iwj30);

  if (scheduler_stop() != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_architecture_scheduler_context_02(void)
{
  i_thread		thread;
  t_thread_context	ctx;
  t_stack		stack;
  o_thread*		t;

  /*
   * thread 1
   */

  if (thread_reserve(_kernel->task, THREAD_PRIORITY, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  stack.base = 0;
  stack.size = THREAD_STACKSZ_LOW;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error");

  if (thread_get(thread, &t) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  ctx.sp = t->stack + t->stacksz - 16;
  ctx.pc = (t_vaddr)test_architecture_scheduler_context_02_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error");

  if (thread_start(thread) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  if (scheduler_start() != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(sdfnwo00fg43kmg0h);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}
