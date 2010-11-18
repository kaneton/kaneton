/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/event/ia32/flags/flags.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [tue nov 16 19:12:37 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "flags.h"

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

void			test_event_ia32_flags_handler(t_id		id)
{
  t_uint32		eflags;

  thrown = 1;

  asm volatile("pushf\n"
               "popl %0"
               : "=g" (eflags));

  printf("[isr] eflags: %s\n",
	 (eflags & (1 << 9) ? "enabled" : "disabled"));
}

void			test_event_ia32_flags_content(void)
{
  t_uint32		eflags;

  if (event_reserve(3,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_flags_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  asm volatile("int $3");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught\n");

  asm volatile("pushf\n"
               "popl %0"
               : "=g" (eflags));

  printf("[content] eflags: %s\n",
	 (eflags & (1 << 9) ? "enabled" : "disabled"));

  // XXX: ici le probleme c'est que c'est un thread et il faudrait
  //      quitter proprement sinon ca va timeout

  while (1)
    ;
}

void			test_event_ia32_flags(void)
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
  ctx.pc = (t_vaddr)test_event_ia32_flags_content;

  if (thread_load(thread, ctx) != ERROR_NONE)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_NONE)
    TEST_ERROR("[ia32_get_context] error\n");

  if (scheduler_add(thread) != ERROR_NONE)
    TEST_ERROR("[scheduler_add] error\n");

  STI();     

  TEST_LEAVE();
}
