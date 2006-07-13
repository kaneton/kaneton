/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/arch/ia32-virtual/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       julien quintard   [sat jul  8 02:30:44 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern --------------------------------------------------------\
--
*/

extern m_thread*	thread;

/*                                                                  [cut] k4 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread manager dispatch.
 */

d_thread			thread_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_thread_stack,
    ia32_thread_load,
    ia32_thread_store,
    ia32_thread_reserve,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 *
 */

t_error			ia32_thread_reserve(i_task		taskid,
					    i_thread*		threadid)
{
  o_task*		task;
  o_as*			as;
  o_thread*		o;

  THREAD_ENTER(thread);

  /*
   *
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(*thread, ERROR_UNKNOWN);

  /*
   *
   */

  if (task_get(taskid, &task) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   *
   */

  if (as_get(task->asid, &as) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * XXX THREAD: verifier d'abord que l'AS existe.
   */

  //  pd_get_cr3(&(o->machdep.context.cr3), as->machdep.pd);

  asm volatile("movl %%cr3, %%eax\n\t"
               "movl %%eax, %0"
               : "=g" (o->machdep.context.cr3)
               :
               : "%eax");
  asm volatile("pushf\n\t"
               "popl %0" : "=g" (o->machdep.context.eflags));

  o->machdep.context.cs = 0x8;
  o->machdep.context.ds = 0x10;
  o->machdep.context.es = 0x10;
  o->machdep.context.fs = 0x10;
  o->machdep.context.gs = 0x10;
  o->machdep.context.ss = 0x10;

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 *
 */

t_error			ia32_thread_stack(i_thread		threadid,
					  t_vsize		size)
{
  o_thread*		o;

  THREAD_ENTER(thread);

  /*
   *
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   *
   */

  //  o->machdep.context.ebp = o->stack;
  o->machdep.context.esp = o->stack;

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 *
 */

t_error			ia32_thread_load(i_thread		threadid,
					 t_thread_context	context)
{
  o_thread*		o;

  THREAD_ENTER(thread);

  /*
   *
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   *
   */

  o->machdep.context.eip = context.pc;
  o->machdep.context.ebp = context.sp;
  o->machdep.context.esp = context.sp;

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 *
 */

t_error			ia32_thread_store(i_thread		threadid,
					  t_thread_context*	context)
{
  o_thread*		o;

  THREAD_ENTER(thread);

  /*
   *
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   *
   */

  context->pc = o->machdep.context.eip;
  context->sp = o->machdep.context.esp;

  THREAD_LEAVE(thread, ERROR_NONE);
}



/*                                                                 [cut] /k4 */
