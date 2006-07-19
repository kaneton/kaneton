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
    ia32_thread_load,
    ia32_thread_store,
    ia32_thread_reserve,
    NULL,
    NULL,
    NULL,
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
  t_uint16		code_segment;
  t_uint16		data_segment;

  THREAD_ENTER(thread);

  /*
   *
   */

  if (thread_get(*threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

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

  pd_get_cr3(&(o->machdep.context.cr3), as->machdep.pd);

  /*
   * XXX THREAD mise en place de la pile
   */

  o->machdep.context.ebp = o->stack;
  o->machdep.context.esp = o->stack;

  /*
   *
   */

  asm volatile("pushf\n\t"
               "popl %0" : "=g" (o->machdep.context.eflags));

  /*
   *
   */

  if (taskid == 0)
    {
      code_segment = 0x8;
      data_segment = 0x10;
    }
  else
    {
      code_segment = 0x18;
      data_segment = 0x20;
    }

  o->machdep.context.cs = code_segment;
  o->machdep.context.ds = data_segment;
  o->machdep.context.es = data_segment;
  o->machdep.context.fs = data_segment;
  o->machdep.context.gs = data_segment;
  o->machdep.context.ss = data_segment;

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
