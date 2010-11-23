/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ine/glue/ibm-pc.ia32/event/clock/clock.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov 20 23:51:04 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "clock.h"

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

t_uint8			test_ibmpc_ia32_event_clock_read(unsigned char	reg)
{
  t_uint8		v;

  OUTB(0x70, reg);
  INB(0x71, v);

  return (v);
}

void			test_ibmpc_ia32_event_clock_write(unsigned char	reg,
							  unsigned char	val)
{
  OUTB(0x70, reg);
  OUTB(0x71, val);
}

void			test_ibmpc_ia32_event_clock_handler(t_id	id)
{
  thrown++;

  asm volatile("movb $0xc, %al\n"
	       "outb %al,$0x70\n"
	       "outb %al,$0x80\n"
	       "inb  $0x71,%al");
}

void			test_ibmpc_ia32_event_clock_content(void)
{
  int			i;

  if (event_reserve(32 + 8,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_ibmpc_ia32_event_clock_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  while (test_ibmpc_ia32_event_clock_read(TEST_RTC_STATUSA) & TEST_RTCSA_TUP)
    ;

  test_ibmpc_ia32_event_clock_write(TEST_RTC_STATUSA,
				    TEST_RTCSA_DIVIDER | TEST_RTCSA_1024);

  test_ibmpc_ia32_event_clock_write(TEST_RTC_STATUSB,
				    test_ibmpc_ia32_event_clock_read(TEST_RTC_STATUSB) |
				    TEST_RTCSB_PINTR | TEST_RTCSB_SQWE);

  test_ibmpc_ia32_event_clock_read(TEST_RTC_INTR);

  for (i = 0; i < 100000000; i++)
    asm volatile("nop");

  if (thrown <= 1)
    TEST_ERROR("one or more timers have not been caught\n");

  while (1)
    ;
}

void			test_ibmpc_ia32_event_clock(void)
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
  ctx.pc = (t_vaddr)test_ibmpc_ia32_event_clock_content;

  if (thread_load(thread, ctx) != ERROR_NONE)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_NONE)
    TEST_ERROR("[ia32_get_context] error\n");

  if (scheduler_add(thread) != ERROR_NONE)
    TEST_ERROR("[scheduler_add] error\n");

  STI();     

  TEST_LEAVE();
}
