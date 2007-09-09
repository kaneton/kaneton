/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/library/libpthread/thread.c
 *
 * created       matthieu bucchianeri   [thu sep  6 21:53:57 2007]
 * updated       matthieu bucchianeri   [mon sep 10 00:13:19 2007]
 */


#include <pthread.h>
#include <crt.h>

int		pthread_create(pthread_t*	th,
			       const void*	attr,
			       void*		(*start_routine)(void*),
			       void*		arg)
{
  t_thread_context	ctx;
  t_stack	stack;
  t_vaddr	stk;

  if (thread_reserve(_crt_get_task_id(), THREAD_PRIOR, th) != ERROR_NONE)
    return (-1);

  stack.base = 0;
  stack.size = 4 * PAGESZ;
  if (thread_stack(*th, stack) != ERROR_NONE)
    return (-1);

  if (thread_attribute_stack(*th, &stack.base) != ERROR_NONE)
    return (-1);

  stk = stack.base + stack.size - 16;

  ctx.sp = stk - 8;
  ctx.pc = (t_uint32)start_routine;
  if (thread_load(*th, ctx) != ERROR_NONE)
    return (-1);

  if (thread_args(*th, &arg, sizeof (void*)) != ERROR_NONE)
    return (-1);

  if (thread_state(*th, SCHEDULER_STATE_RUN) != ERROR_NONE)
    return (-1);

  return 0;
}

void	pthread_yield(void)
{
#if 0
  i_cpu	c;

  if (cpu_current(&c) == ERROR_NONE)
    scheduler_yield(c);
#endif
  scheduler_yield(0);
}
