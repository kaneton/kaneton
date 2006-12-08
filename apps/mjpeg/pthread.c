/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/kaneton/apps/mjpeg/pthread.c
 *
 * created       matthieu bucchianeri   [sat dec  2 02:27:13 2006]
 * updated       matthieu bucchianeri   [fri dec  8 02:21:49 2006]
 */

#include <pthread.h>

extern i_task		ktask;

int		pthread_create(pthread_t*	th,
			       const void*	attr,
			       void*		(*start_routine)(void*),
			       void*		arg)
{
  t_thread_context	ctx;
  t_stack	stack;
  o_thread	*o;
  t_uint32	*stk;

  if (thread_reserve(ktask, THREAD_PRIOR, th) != ERROR_NONE)
    return -1;

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;
  if (thread_stack(*th, stack) != ERROR_NONE)
    return -1;
  if (thread_get(*th, &o) != ERROR_NONE)
    return -1;

  stk = o->stack;

  *--stk = arg;
  *--stk = 0;
  *stk = 0;
  ctx.sp = stk;
  ctx.pc = (t_uint32)start_routine;
  if (thread_load(*th, ctx) != ERROR_NONE)
    return -1;

  if (thread_state(*th, SCHED_STATE_RUN) != ERROR_NONE)
    return -1;

  return 0;
}

void	pthread_yield(void)
{
  sched_yield(0);
}
