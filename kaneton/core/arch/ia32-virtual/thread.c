/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       matthieu bucchianeri   [wed jul 26 12:15:34 2006]
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

extern i_as		kasid;

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
    ia32_thread_init
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * clone the ia32 architecture dependent part of a thread.
 *
 * steps:
 *
 * 1) get the thread to clone from the threads container.
 * 2) get the new thread from the threads container.
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
 * reserve a thread on the ia32 architecture
 *
 * steps:
 *
 * 1) get the thread object for the specified thread.
 * 2) get the task object the thread belongs to.
 * 3) get the task's address space.
 * 4)
 * 5)
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
   * 1)
   */

  if (thread_get(*threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(taskid, &task) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_get(task->asid, &as) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);


  /*
   * 4)
   */

  memset(&(o->machdep.context), 0x0, sizeof(t_ia32_context));

  pd_get_cr3(&(o->machdep.context.cr3), as->machdep.pd);

  SEFLAGS(o->machdep.context.eflags);

  /*
   * 5)
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

  if (cpucaps & IA32_CAPS_SSE)
    memset(&o->machdep.u.sse, 0, sizeof(t_sse_state));
  else
    memset(&o->machdep.u.x87, 0, sizeof(t_x87_state));

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

/*
 *
 *
 * steps:
 *
 * 1) get the kernel address space.
 * 2) fill the tss.
 * 3) load the current tss.
 */

t_error			ia32_thread_init(void)
{
  THREAD_ENTER(thread);

  o_as*			as;

  /*
   * 1)
   */


  if (as_get(kasid, &as) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  memset(&(thread->machdep.tss), 0x0, sizeof(t_ia32_tss));

  thread->machdep.tss.ss = SEGSEL(PMODE_GDT_CORE_DS, PRIV_RING0);

  pd_get_cr3(&(thread->machdep.tss.cr3), as->machdep.pd);

  SEFLAGS(thread->machdep.tss.eflags);

  /*
   * 3)
   */

  tss_init(&(thread->machdep.tss));

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*                                                                 [cut] /k4 */
