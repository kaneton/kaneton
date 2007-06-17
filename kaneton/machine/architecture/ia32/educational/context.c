/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/architecture/ia32/educational/context.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       matthieu bucchianeri   [fri jun 15 09:30:33 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * stores the interrupted context.
 */

t_ia32_context*	ia32_context = NULL;

/*
 * tells the processor capabilities.
 */

t_uint32	ia32_cpucaps = 0;

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_thread*	thread;
extern i_as		kasid;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function set the pdbr value to all the threads in a task.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) get the destination task.
 * 3) for each thread, update the context's pdbr value.
 */

t_error			ia32_update_pdbr(i_task			tskid,
					 i_as			asid)
{
  o_as*			o;
  o_task*		otask;
  o_thread*		oth;
  t_iterator		it;
  t_state		state;

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(tskid, &otask) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, otask->threads, &it, state)
    {
      i_thread*		th;

      if (set_object(otask->threads, it, (void**)&th) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (thread_get(*th, &oth) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (ia32_pd_get_cr3(&oth->machdep.context.cr3, o->machdep.pd,
			  IA32_PD_CACHED, IA32_PD_WRITEBACK) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  return (ERROR_NONE);
}

/*
 * initialize a few things about extended contexts.
 *
 * steps:
 *
 * 1) initialize the FPU.
 * 2) detect MMX and SSE processor capabilities.
 */

t_error			ia32_extended_context_init(void)
{
  /*
   * 1)
   */

  FINIT();

  /*
   * 2)
   */

  if (ia32_cpu_has_mmx())
    ia32_cpucaps |= IA32_CAPS_MMX;

  if (ia32_cpu_has_sse() && ia32_cpu_has_fxstate())
    {
      ia32_enable_sse();
      ia32_cpucaps |= IA32_CAPS_SSE;
    }

  return (ERROR_NONE);
}

/*
 * this function copies a context. useful for context switching.
 */

void			ia32_context_copy(t_ia32_context*		dst,
					  const t_ia32_context*		src)
{
  dst->eax = src->eax;
  dst->ebx = src->ebx;
  dst->ecx = src->ecx;
  dst->edx = src->edx;
  dst->esi = src->esi;
  dst->edi = src->edi;
  dst->esp = src->esp;
  dst->ebp = src->ebp;
  dst->eip = src->eip;
  dst->cr3 = src->cr3;
  dst->cs = src->cs;
  dst->ds = src->ds;
  dst->es = src->es;
  dst->fs = src->fs;
  dst->gs = src->gs;
  dst->ss = src->ss;
  dst->eflags = src->eflags;
}

/*
 * this functions clears the io permission bitmap of a task.
 */

t_error			ia32_clear_io_bitmap(i_task		tskid)
{
  o_task*		o;

  if (task_get(tskid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  memset(&o->machdep.iomap, 0xFF, 8192);

  return (ERROR_NONE);
}

/*
 * this function duplicate the io permission bitmap of a task to another.
 */

t_error			ia32_duplicate_io_bitmap(i_task		old,
						 i_task		new)
{
  o_task*		from;
  o_task*		to;

  if (task_get(old, &from) != ERROR_NONE || task_get(new, &to) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  memcpy(to->machdep.iomap, from->machdep.iomap, 8192);

  return (ERROR_NONE);
}

/*
 * this function modifies a bit in a bitmap.
 */

static void		io_bitmap_set(t_uint8*		bitmap,
				      t_uint32		port,
				      t_uint32		value)
{
  if (value)
    bitmap[port / 8] |= (1 << (port % 8));
  else
    bitmap[port / 8] &= ~(1 << (port % 8));
}

/*
 * this function checks one or more bits in a bitmap. bits to '1' have
 * higher precedence.
 */

static t_uint8		io_bitmap_isset(t_uint8*	bitmap,
					t_uint32	port,
					t_uint8		consecutive)
{
  t_uint8		i;

  for (i = 0 ; i < consecutive; i++)
    if ((bitmap[port / 8] & (1 << (port % 8))) != 0)
      return 1;
  return 0;
}

/*
 * this function allow I/O to a port.
 *
 * steps:
 *
 * 1) check the port id.
 * 2) get the task.
 * 3) change permission bitmap.
 */

t_error			ia32_set_io_bitmap(i_task		tskid,
					   i_port		id,
					   t_uint8		width,
					   t_uint8		allow)
{
  o_task*		o;
  t_uint8		i;

  /*
   * 1)
   */

  if (id + width >= 65536)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(tskid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  for (i = 0; i < width; i++)
    io_bitmap_set(o->machdep.iomap, id + i, !allow);

  return (ERROR_NONE);
}

/*
 * setup current I/O PL to 0.
 */

t_error			ia32_reset_iopl(void)
{
  asm volatile("pushf\n\t"
	       "andl $0xFFFFCFFF, %ss:(%esp)\n\t"
	       "popf");

  return (ERROR_NONE);
}

/*
 * this function initializes a blank context.
 *
 * steps:
 *
 * 1) get the thread object for the specified thread.
 * 2) get the task object the thread belongs to.
 * 3) get the task's address space.
 * 4) reset the context to zero.
 * 5) setup the pdbr and the eflags in the context.
 * 6) depending on the task class, update the segment selectors into
 *    the context.
 * 7) reset advanced context (FPU or SSE).
 */

t_error			ia32_init_context(i_task		taskid,
					  i_thread		threadid)
{
  o_task*		task;
  o_as*			as;
  o_thread*		o;
  t_uint16		code_segment;
  t_uint16		data_segment;

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(taskid, &task) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  if (as_get(task->asid, &as) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 4)
   */

  memset(&(o->machdep.context), 0x0, sizeof(t_ia32_context));

  /*
   * 5)
   */

  ia32_pd_get_cr3(&(o->machdep.context.cr3), as->machdep.pd,
		  IA32_PD_CACHED, IA32_PD_WRITEBACK);

  o->machdep.context.eflags = (1 << 9) | (1 << 1);

  /*
   * 6)
   */

  switch(task->class)
    {
      case TASK_CLASS_CORE:
	code_segment = IA32_SEGSEL(IA32_PMODE_GDT_CORE_CS, IA32_PRIV_RING0);
	data_segment = IA32_SEGSEL(IA32_PMODE_GDT_CORE_DS, IA32_PRIV_RING0);
	break;
      case TASK_CLASS_DRIVER:
	code_segment = IA32_SEGSEL(IA32_PMODE_GDT_DRIVER_CS, IA32_PRIV_RING1);
	data_segment = IA32_SEGSEL(IA32_PMODE_GDT_DRIVER_DS, IA32_PRIV_RING1);
	o->machdep.context.eflags |= (1 << 12);
	break;
      case TASK_CLASS_SERVICE:
	code_segment = IA32_SEGSEL(IA32_PMODE_GDT_SERVICE_CS, IA32_PRIV_RING2);
	data_segment = IA32_SEGSEL(IA32_PMODE_GDT_SERVICE_DS, IA32_PRIV_RING2);
	break;
      default:
	code_segment = IA32_SEGSEL(IA32_PMODE_GDT_PROGRAM_CS, IA32_PRIV_RING3);
	data_segment = IA32_SEGSEL(IA32_PMODE_GDT_PROGRAM_DS, IA32_PRIV_RING3);
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

  return (ERROR_NONE);
}


/*
 * this function duplicate the whole context of a thread.
 */

t_error			ia32_duplicate_context(i_thread		old,
					       i_thread		new)
{
  o_thread*		from;
  o_thread*		to;

  if (thread_get(old, &from) != ERROR_NONE ||
      thread_get(new, &to) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  memcpy(&(to->machdep.context), &(from->machdep.context),
	 sizeof(t_ia32_context));

  if (cpucaps & IA32_CAPS_SSE)
    memcpy(&to->machdep.u.sse, &from->machdep.u.sse, sizeof(t_sse_state));
  else
    memcpy(&to->machdep.u.x87, &from->machdep.u.x87, sizeof(t_x87_state));

  return (ERROR_NONE);
}

/*
 * this function updates the context with the new stack and
 * instruction pointers.
 */

t_error			ia32_setup_context(i_thread		threadid,
					   t_vaddr		pc,
					   t_vaddr		sp)
{
  o_thread*		o;

  if (thread_get(threadid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  o->machdep.context.eip = pc;
  o->machdep.context.esp = sp;

  return (ERROR_NONE);
}

/*
 * this function reads from the context both the stack and instruction
 * pointers.
 */

t_error			ia32_status_context(i_thread		threadid,
					    t_vaddr*		pc,
					    t_vaddr*		sp)
{
  o_thread*		o;

  if (thread_get(threadid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  *pc = o->machdep.context.eip;
  *sp = o->machdep.context.esp;

  return (ERROR_NONE);
}

/*
 * initialize the ia32 context switch mecanism.
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

t_error			ia32_init_switcher(void)
{
  o_as*			as;
  t_vaddr		int_stack;

  /*
   * 1)
   */

  if (as_get(kasid, &as) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (map_reserve(kasid,
		  MAP_OPT_PRIVILEGED,
		  3 * PAGESZ,
		  PERM_READ | PERM_WRITE,
		  (t_vaddr*)&thread->machdep.tss) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  memset(thread->machdep.tss, 0x0, sizeof(t_ia32_tss));

  /*
   * 3)
   */

  if (map_reserve(kasid,
		  MAP_OPT_PRIVILEGED,
		  2 * PAGESZ,
		  PERM_READ | PERM_WRITE,
		  &int_stack) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 4)
   */

  if (ia32_tss_load(thread->machdep.tss,
		    IA32_SEGSEL(IA32_PMODE_GDT_CORE_DS, IA32_PRIV_RING0),
		    int_stack + 2 * PAGESZ - 16,
		    0x68) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 5)
   */

  if (ia32_tss_init(thread->machdep.tss) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  return (ERROR_NONE);
}

/*
 * this function switches execution to the specified thread.
 *
 * steps:
 *
 * 1) check if a switch is necessary.
 * 2) store back the executing context into the executing thread.
 * 3) set current context as the elected thread one.
 * 4) update the I/O permissions bit map.
 * 5) set the TS flag so any use of FPU, MMX or SSE instruction will
 *    generate an exception.
 */

t_error			ia32_context_switch(i_thread		current,
					    i_thread		elected)
{
  o_thread*		from;
  o_thread*		to;
  o_task*		task;

  if (!context)
    {
      cons_msg('!', "unable to switch context in this state\n");

      while (1)
	;
    }

  /*
   * 1)
   */

  if (current == elected)
      return (ERROR_NONE);

  /*
   * 2)
   */

  if (current != ID_UNUSED)
    {
      if (thread_get(current, &from) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      ia32_context_copy(&from->machdep.context, context);
    }

  /*
   * 3)
   */

  if (thread_get(elected, &to) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  ia32_context_copy(context, &to->machdep.context);

  /*
   * 4)
   */

  if (task_get(to->taskid, &task) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  memcpy((t_uint8*)thread->machdep.tss + thread->machdep.tss->io,
	 &task->machdep.iomap,
	 8192);

  /*
   * 5)
   */

  STS();

  return (ERROR_NONE);
}

/*
 * this function is used to switch FPU, MMX and SSE context.
 *
 * steps:
 *
 * 1) get both executing and old contexts.
 * 2) SSE case
 *  a) save the SSE & FPU context into the previous thread context.
 *  b) restore the SSE & FPU context of the executing thread.
 * 3) FPU or MMX case
 *  a) save the FPU/MMX context into the previous thread context.
 *  b) restore the FPU/MMX context of the executing thread.
 * 4) clears the TS bit in CR0.
 */

t_error			ia32_extended_context_switch(i_thread	current,
						     i_thread	elected)
{
  o_thread*		o;
  o_thread*		old;

  /*
   * 1)
   */


  if (thread_get(elected, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (thread_get(current, &old) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (cpucaps & IA32_CAPS_SSE)
    {
      /*
       * 2)
       */

      /*
       * a)
       */

      FXSAVE(old->machdep.u.sse);

      /*
       * b)
       */

      FXRSTOR(o->machdep.u.sse);
    }
  else
    {
      /*
       * 3)
       */

      /*
       * a)
       */

      FSAVE(old->machdep.u.x87);

      /*
       * b)
       */

      FRSTOR(o->machdep.u.x87);
    }

  /*
   * 4)
   */

  CLTS();

  return (ERROR_NONE);
}
