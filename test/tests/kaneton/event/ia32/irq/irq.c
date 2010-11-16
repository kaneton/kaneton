/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../test/tests/kaneton/event/ia32/irq/irq.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [tue nov 16 18:58:31 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "irq.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_task		ktask;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int	thrown = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_event_ia32_irq_handler(t_id		id)
{
  thrown++;
}

void			test_event_ia32_irq_content(void)
{
  int			i;

  if (event_reserve(32 + 6,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_irq_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  OUTB(0x3F0 + 2, 0);
  for (i = 0; i < 10000; i++)
    asm volatile("nop");

  OUTB(0x3F0 + 2, 0x0C);
  for (i = 0; i < 10000000; i++)
    asm volatile("nop");

  OUTB(0x3F0 + 2, 0);
  for (i = 0; i < 10000; i++)
    asm volatile("nop");

  OUTB(0x3F0 + 2, 0x0C);
  for (i = 0; i < 10000000; i++)
    asm volatile("nop");

  if (thrown != 2)
    TEST_ERROR("one or more IRQs have not been caught\n");

  while (1)
    ;
}

void			test_event_ia32_irq(void)
{
  i_thread		thread;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  TEST_ENTER();

  if (thread_reserve(ktask, THREAD_PRIOR, &thread) != ERROR_NONE)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_NONE)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &o) != ERROR_NONE)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_event_ia32_irq_content;

  if (thread_load(thread, ctx) != ERROR_NONE)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_NONE)
    TEST_ERROR("[ia32_get_context] error\n");

  if (scheduler_add(thread) != ERROR_NONE)
    TEST_ERROR("[scheduler_add] error\n");

  STI();     

  TEST_LEAVE();
}
