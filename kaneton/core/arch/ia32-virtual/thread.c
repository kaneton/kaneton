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
    ia32_thread_clone,
    NULL,
    ia32_thread_load,
    ia32_thread_store,
    ia32_thread_reserve,
    NULL,
    NULL,
    NULL,
    ia32_thread_stack,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * clone the ia32 architecture dependant part of a thread.
 *
 * steps:
 *
 * 1) get the thread to clone from the threads container.
 * 2) get the new thread from the threads containers.
 * 3) copy the ia32-dependent object data.
 */

t_error			ia32_thread_clone(i_task		taskid,
					  i_thread		old,
					  i_thread*		new)
{
  o_thread*		from;
  o_thread*		to;

  THREAD_ENTER(thread);

  /*
   * 1)
   */

  if (thread_get(old, &from) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (thread_get(*new, &to) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 3)
   */

  memcpy(&(to->machdep.context), &(from->machdep.context),
	 sizeof(t_ia32_context));

  THREAD_LEAVE(thread, ERROR_NONE)
}

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
   *
   */

  SEFLAGS(o->machdep.context.eflags);

  /*
   *
   */

  switch(task->class)
    {
      case TASK_CLASS_CORE:
	code_segment = SEGSEL(PMODE_GDT_CORE_CS, PRIV_RING0);
	data_segment = SEGSEL(PMODE_GDT_CORE_DS, PRIV_RING0);
	break;
      case TASK_CLASS_DRIVER:
	code_segment = SEGSEL(PMODE_GDT_DRIVER_CS, PRIV_RING1);
	data_segment = SEGSEL(PMODE_GDT_DRIVER_DS, PRIV_RING1);
	break;
      case TASK_CLASS_SERVICE:
	code_segment = SEGSEL(PMODE_GDT_SERVICE_CS, PRIV_RING2);
	data_segment = SEGSEL(PMODE_GDT_SERVICE_DS, PRIV_RING2);
	break;
      default:
	code_segment = SEGSEL(PMODE_GDT_PROGRAM_CS, PRIV_RING3);
	data_segment = SEGSEL(PMODE_GDT_PROGRAM_DS, PRIV_RING3);
    }

  o->machdep.context.cs = code_segment;
  o->machdep.context.ds = data_segment;
  o->machdep.context.es = data_segment;
  o->machdep.context.fs = data_segment;
  o->machdep.context.gs = data_segment;
  o->machdep.context.ss = data_segment;

  /*
   *
   */

  o->machdep.context.esi = 0;
  o->machdep.context.edi = 0;
  o->machdep.context.edx = 0;
  o->machdep.context.ecx = 0;
  o->machdep.context.ebx = 0;
  o->machdep.context.eax = 0;

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
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  context->pc = o->machdep.context.eip;
  context->sp = o->machdep.context.esp;

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * setup the stack on ia32 architecture.
 *
 * steps:
 *
 * 1) get the thread object fromm the threads container.
 * 2) update the machine-dependent part of the object.
 */

t_error			ia32_thread_stack(i_thread		threadid,
					  t_stack		stack)
{
  o_thread*		o;

  THREAD_ENTER(thread);

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  o->machdep.context.ebp = stack.base + stack.size - 1;

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*                                                                 [cut] /k4 */
