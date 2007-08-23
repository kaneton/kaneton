/*
 * licence       kaneton licence
 *
 * project       kaneton-mjpeg
 *
 * file          /home/buckman/kaneton/apps/mjpeg/pthread.c
 *
 * created       matthieu bucchianeri   [sat dec  2 02:27:13 2006]
 * updated       matthieu bucchianeri   [wed dec 13 15:15:50 2006]
 */

#include <pthread.h>
#include <crt.h>
#include <libc.h>

int		pthread_create(pthread_t*	th,
			       const void*	attr,
			       void*		(*start_routine)(void*),
			       void*		arg)
{
  t_thread_context	ctx;
  t_stack	stack;
  t_vaddr	stk;
  static int no = 0;

  if (thread_reserve(_crt_get_task_id(), THREAD_PRIOR, th) != ERROR_NONE)
    {
      printf("cannot thread_reserve\n");

      while (1)
	;
    }

  no ++;

  stack.base = 0;
  stack.size = 4 * PAGESZ;
  if (thread_stack(*th, stack) != ERROR_NONE)
    {
      printf("cannot thread_stack\n");

      while (1)
	;
    }

  if (thread_attribute_stack(*th, &stack.base) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  stk = stack.base + stack.size - 16;

  ctx.sp = stk - 8;
  ctx.pc = (t_uint32)start_routine;
  if (thread_load(*th, ctx) != ERROR_NONE)
    {
      printf("cannot thread_load\n");

      while (1)
	;
    }

  if (thread_args(*th, &arg, sizeof (void*)) != ERROR_NONE)
    {
      printf("cannot thread_args\n");

      while (1)
	;
    }

  return 0;
}
