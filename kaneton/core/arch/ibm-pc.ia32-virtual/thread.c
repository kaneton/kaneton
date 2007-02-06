/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/thread.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       matthieu bucchianeri   [tue feb  6 23:14:26 2007]
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
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the thread manager.
 */

extern m_thread*	thread;

/*                                                                  [cut] k3 */

/*
 * we'll use the kernel address space identifier.
 */

extern i_as		kasid;

/*                                                                 [cut] /k3 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the thread manager dispatch.
 */

d_thread			thread_dispatch =
  {

/*                                                                  [cut] k3 */

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
    ia32_thread_init,
    NULL

/*                                                                 [cut] /k3 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k3 */

/*
 * clone the ia32 architecture dependent part of a thread.
 *
 * steps:
 *
 * 1) get both the original and the cloned thread objects.
 * 2) copy the ia32-dependent object data, including the general
 *    context and the extended registers
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

  if (thread_get(old, &from) != ERROR_NONE ||
      thread_get(*new, &to) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  memcpy(&(to->machdep.context), &(from->machdep.context),
	 sizeof(t_ia32_context));

  if (cpucaps & IA32_CAPS_SSE)
    memcpy(&to->machdep.u.sse, &from->machdep.u.sse, sizeof(t_sse_state));
  else
    memcpy(&to->machdep.u.x87, &from->machdep.u.x87, sizeof(t_x87_state));

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
 * 4) reset the context to zero.
 * 5) setup the PDBR and the EFLAGS in the context.
 * 6) depending on the task class, update the segment selectors into
 *    the context.
 * 7) reset advanced context (FPU or SSE).
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

  /*
   * 5)
   */

  pd_get_cr3(&(o->machdep.context.cr3), as->machdep.pd,
	     PD_CACHED, PD_WRITEBACK);

  o->machdep.context.eflags = (1 << 9) | (1 << 1);

  /*
   * 6)
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
   * 7)
   */

  if (cpucaps & IA32_CAPS_SSE)
    memset(&o->machdep.u.sse, 0, sizeof(t_sse_state));
  else
    memset(&o->machdep.u.x87, 0, sizeof(t_x87_state));

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * this function updates the context with the new stack and
 * instruction pointers.
 */

t_error			ia32_thread_load(i_thread		threadid,
					 t_thread_context	context)
{
  o_thread*		o;

  THREAD_ENTER(thread);

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  o->machdep.context.eip = context.pc;
  o->machdep.context.esp = context.sp;

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*
 * this function reads from the context both the stack and instruction
 * pointers.
 */

t_error			ia32_thread_store(i_thread		threadid,
					  t_thread_context*	context)
{
  o_thread*		o;

  THREAD_ENTER(thread);

  if (thread_get(threadid, &o) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

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

  o->machdep.context.ebp = o->stack + o->stacksz - 16;

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*                                                                 [cut] /k3 */

/*
 * initialize the IA-32 related structures for the thread manager.
 *
 * steps:
 *
 * 1) get the kernel address space.
 * 2) reserve some memory for the TSS.
 * 3) reserve 2 pages for the interrupt stack. this stack is
 *    automatically setup by the processor on privilege level switch.
 * 4) load the interrupt stack and the I/O bitmap base into the TSS.
 * 5) activate the TSS.
 */

t_error			ia32_thread_init(void)
{
  THREAD_ENTER(thread);

  o_as*			as;
  t_vaddr		int_stack;

  /*
   * 1)
   */

  if (as_get(kasid, &as) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (map_reserve(kasid,
		  MAP_OPT_PRIVILEGED,
		  3 * PAGESZ,
		  PERM_READ | PERM_WRITE,
		  (t_vaddr*)&thread->machdep.tss) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  memset(thread->machdep.tss, 0x0, sizeof(t_ia32_tss));

  /*
   * 3)
   */

  if (map_reserve(kasid,
		  MAP_OPT_PRIVILEGED,
		  2 * PAGESZ,
		  PERM_READ | PERM_WRITE,
		  &int_stack) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (tss_load(thread->machdep.tss,
	       SEGSEL(PMODE_GDT_CORE_DS, PRIV_RING0),
	       int_stack + 2 * PAGESZ - 16,
	       0x68) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (tss_init(thread->machdep.tss) != ERROR_NONE)
    THREAD_LEAVE(thread, ERROR_UNKNOWN);

  THREAD_LEAVE(thread, ERROR_NONE);
}

/*                                                                 [cut] /k4 */
