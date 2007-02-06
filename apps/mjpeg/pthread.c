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

extern i_task mjpeg_task;
extern i_as mjpeg_asid;
extern i_as kasid;

int		pthread_create(pthread_t*	th,
			       const void*	attr,
			       void*		(*start_routine)(void*),
			       void*		arg)
{
  t_thread_context	ctx;
  t_stack	stack;
  o_thread	*o;
  t_vaddr	stk;
  static int no = 0;

  if (thread_reserve(mjpeg_task, no == 7 ? THREAD_PRIOR : THREAD_HPRIOR, th) != ERROR_NONE)
    {
      printf("cannot thread_reserve\n");

      while (1)
	;
    }

  no ++;

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;
  if (thread_stack(*th, stack) != ERROR_NONE)
    {
      printf("cannot thread_stack\n");

      while (1)
	;
    }
  if (thread_get(*th, &o) != ERROR_NONE)
    {
      printf("cannot thread_get\n");

      while (1)
	;
    }

  stk = o->stack + o->stacksz - 16;

  t_paddr seg;
  i_region reg;

  as_paddr(mjpeg_asid, o->stack, &seg);

  if (region_reserve(kasid, (i_segment)seg, 0, REGION_OPT_NONE, 0, o->stacksz, &reg) != ERROR_NONE)
    {
      printf("cannot region_reserve\n");

      while (1)
	;
    }

  tlb_flush();

//  printf("stack top @ %p (%p) of size %u\n", stk, seg, stack.size);

  t_uint32 *top = (t_uint32 *)(reg + o->stacksz - 16);

//  printf("mapped @ %p\n", top);
//  printf("arg = %p\n", arg);
  *(--top) = arg;
  *(--top) = 42;
  *(--top) = 43;
  *(--top) = 44;
  *(--top) = 45;

  region_release(kasid, reg);

  ctx.sp = stk - 8;
  ctx.pc = (t_uint32)start_routine;
  if (thread_load(*th, ctx) != ERROR_NONE)
    {
      printf("cannot thread_load\n");

      while (1)
	;
    }

  return 0;
}

void	pthread_yield(void)
{
//  sched_yield(0);
  asm volatile("int $31");
}
