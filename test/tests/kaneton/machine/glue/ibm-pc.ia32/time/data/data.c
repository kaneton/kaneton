/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...achine/glue/ibm-pc.ia32/time/data/data.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sat nov 20 23:41:18 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "data.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		timed = 0;
static volatile i_timer		tid;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_ibmpc_ia32_time_data_handler(t_id		id,
							t_vaddr		data)
{
  timed = 1;

  if (tid != id)
    TEST_ERROR("invalid timer's identifier\n");

  if (data != 0x41414141)
    TEST_ERROR("invalid timer's data\n");
}

void			test_ibmpc_ia32_time_data_content(void)
{
  t_uint32		start;

  // XXX
  while (1)
    {
      int start;

      start = test_ibmpc_ia32_time_cmos();
      while (((start + 3) % 60) != test_ibmpc_ia32_time_cmos())
	;

      printf("[thread] child: %d\n", test_ibmpc_ia32_time_cmos());
    }

  start = test_ibmpc_ia32_time_cmos();
  while (start == test_ibmpc_ia32_time_cmos())
    ;

  if (timer_reserve(TIMER_FUNCTION,
		    TIMER_HANDLER(test_ibmpc_ia32_time_data_handler),
		    0x41414141,
		    500,
		    TIMER_REPEAT_DISABLE,
		    (i_timer*)&tid) != ERROR_OK)
    TEST_ERROR("[event_reserve] error\n");

  start = test_ibmpc_ia32_time_cmos();
  while (((start + 3) % 60) != test_ibmpc_ia32_time_cmos() &&
	 !timed)
    ;

  if (timed != 1)
    TEST_ERROR("the timer has not been triggered\n");

  while (1)
    ;
}

void			test_ibmpc_ia32_time_data(void)
{
  i_thread		thread;
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;
  t_ia32_context	ia32_ctx;

  TEST_ENTER();

  if (thread_reserve(_kernel->task, THREAD_PRIOR, &thread) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error\n");

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thread, stack) != ERROR_OK)
    TEST_ERROR("[thread_stack] error\n");

  if (thread_get(thread, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error\n");

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)test_ibmpc_ia32_time_data_content;

  if (thread_load(thread, ctx) != ERROR_OK)
    TEST_ERROR("[thread_load] error\n");

  if (ia32_get_context(thread, &ia32_ctx) != ERROR_OK)
    TEST_ERROR("[ia32_get_context] error\n");

  /* XXX il manque ici set eflags puis ia32_set_context: voir kaneton/check/common/common.c */

  if (scheduler_add(thread) != ERROR_OK)
    TEST_ERROR("[scheduler_add] error\n");

  STI();     

  printf("HERE\n");

  // XXX
  while (1)
    {
      int start;

      start = test_ibmpc_ia32_time_cmos();
      while (((start + 3) % 60) != test_ibmpc_ia32_time_cmos())
	;

      printf("[thread] main\n");
    }

  TEST_LEAVE();
}
