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

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * tells the processor capabilities.
 */

t_uint32	ia32_cpucaps = 0;

/*
 * stack switching addresses
 */

IA32_HANDLER_DATA_SECTION t_ia32_cpu_local	ia32_local_interrupt_stack = 0;
IA32_HANDLER_DATA_SECTION t_ia32_cpu_local	ia32_local_jump_stack = 0;
IA32_HANDLER_DATA_SECTION t_ia32_cpu_local	ia32_local_jump_pdbr = 0;

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_thread*	thread;
extern i_as		kasid;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * initialize a few things about extended contexts.
 *
 * steps:
 *
 * 1) initialize the FPU.
 * 2) detect MMX and SSE processor capabilities.
 */

t_status		ia32_extended_context_init(void)
{
  /*
   * 1)
   */

  CLTS();	/* XXX broadcast to AP */

  FINIT();	/* XXX broadcast to AP */

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

  return (STATUS_OK);
}

/*
 * this functions clears the io permission bitmap of a task.
 */

t_status		ia32_clear_io_bitmap(i_task		tskid)
{
  o_task*		o;

  if (task_get(tskid, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  memset(&o->machine.iomap, 0xFF, 8192);

  return (STATUS_OK);
}

/*
 * this function duplicate the io permission bitmap of a task to another.
 */

t_status		ia32_duplicate_io_bitmap(i_task		old,
						 i_task		new)
{
  o_task*		from;
  o_task*		to;

  if (task_get(old, &from) != STATUS_OK || task_get(new, &to) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  memcpy(to->machine.iomap, from->machine.iomap, 8192);

  return (STATUS_OK);
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

t_status		ia32_set_io_bitmap(i_task		tskid,
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
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  if (task_get(tskid, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 3)
   */

  for (i = 0; i < width; i++)
    io_bitmap_set(o->machine.iomap, id + i, !allow);

  o->machine.ioflush = 1;

  /* XXX beurk */
#if 0
  memcpy((t_uint8*)thread->machine.tss + thread->machine.tss->io,
	 &o->machine.iomap,
	 8192);
#endif

  return (STATUS_OK);
}

/*
 * setup current I/O PL to 0.
 */

t_status		ia32_reset_iopl(void)
{
  asm volatile("pushf\n\t"
	       "andl $0xFFFFCFFF, %ss:(%esp)\n\t"
	       "popf");

  return (STATUS_OK);
}

/*
 * this function initializes a blank context.
 *
 * steps:
 *
 * 1) get the thread object for the specified thread.
 * 2) get the task object the thread belongs to.
 * 3) reserve the thread privileged stack.
 * 4) reset the context to zero and initialize constants.
 * 5) reset advanced context (FPU or SSE).
 */

t_status		ia32_init_context(i_task		taskid,
					  i_thread		threadid)
{
  o_thread*		o;
  o_task*		task;
  t_ia32_context	ctx;


  /*
   * 1)
   */

  if (thread_get(threadid, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  if (task_get(taskid, &task) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 3)
   */

  /* XXX can be deleted after first cs when task->class == TASK_CLASS_KERNEL*/
  if (map_reserve(task->asid, MAP_OPT_NONE, PAGESZ, PERM_READ | PERM_WRITE,
		  &o->machine.interrupt_stack) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  o->machine.interrupt_stack += (PAGESZ - 16);

  /*
   * 4)
   */

  memset(&ctx, 0, sizeof (t_ia32_context));
  ctx.eflags = (1 << 9) | (1 << 1);
  if (task->class == TASK_CLASS_DRIVER)
    ctx.eflags |= (1 << 12);

  switch (task->class)
    {
      case TASK_CLASS_KERNEL:
	ctx.ds = ctx.ss = thread->machine.core_ds;
	ctx.cs = thread->machine.core_cs;
	break;
      case TASK_CLASS_DRIVER:
	ctx.ds = ctx.ss = thread->machine.driver_ds;
	ctx.cs = thread->machine.driver_cs;
	break;
      case TASK_CLASS_SERVICE:
	ctx.ds = ctx.ss = thread->machine.service_ds;
	ctx.cs = thread->machine.service_cs;
	break;
      case TASK_CLASS_GUEST:
	ctx.ds = ctx.ss = thread->machine.program_ds;
	ctx.cs = thread->machine.program_cs;
	break;
    }

  if (ia32_set_context(threadid, &ctx, IA32_CONTEXT_FULL) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 5)
   */

  if (ia32_cpucaps & IA32_CAPS_SSE)
    {
      memset(&o->machine.u.sse, 0, sizeof (t_sse_state));

      o->machine.u.sse.fcw = 0x37f;
      o->machine.u.sse.ftw = 0xffff;
    }
  else
    {
      memset(&o->machine.u.x87, 0, sizeof (t_x87_state));

      o->machine.u.x87.fcw = 0x37f;
      o->machine.u.x87.ftw = 0xffff;
    }

  return (STATUS_OK);
}


/*
 * this function duplicate the whole context of a thread.
 */

t_status		ia32_duplicate_context(i_thread		old,
					       i_thread		new)
{
  o_thread*		from;
  o_thread*		to;
  t_ia32_context	ctx;

  if (thread_get(old, &from) != STATUS_OK ||
      thread_get(new, &to) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (ia32_get_context(old, &ctx) != STATUS_OK ||
      ia32_set_context(new, &ctx, IA32_CONTEXT_FULL) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (ia32_cpucaps & IA32_CAPS_SSE)
    memcpy(&to->machine.u.sse, &from->machine.u.sse, sizeof (t_sse_state));
  else
    memcpy(&to->machine.u.x87, &from->machine.u.x87, sizeof (t_x87_state));

  return (STATUS_OK);
}

/*
 * this function updates the context with the new stack and
 * instruction pointers.
 */

t_status		ia32_setup_context(i_thread		threadid,
					   t_vaddr		pc,
					   t_vaddr		sp)
{
  t_ia32_context	ctx;

  ctx.eip = pc;
  ctx.esp = sp;

  return (ia32_set_context(threadid, &ctx,
			   IA32_CONTEXT_EIP | IA32_CONTEXT_ESP));
}

/*
 * this function reads from the context both the stack and instruction
 * pointers.
 */

t_status		ia32_status_context(i_thread		threadid,
					    t_vaddr*		pc,
					    t_vaddr*		sp)
{
  t_ia32_context	ctx;

  assert(pc != NULL);
  assert(sp != NULL);

  if (ia32_get_context(threadid, &ctx) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  *pc = ctx.eip;
  *sp = ctx.esp;

  return (STATUS_OK);
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

t_status		ia32_init_switcher(void)
{
  o_as*			as;
  i_segment		seg;
  i_region		reg;
  t_vaddr		int_stack;

  /*
   * 1)
   */

  if (as_get(kasid, &as) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  if (segment_reserve(kasid, 3 * PAGESZ, PERM_READ | PERM_WRITE,
		      &seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (region_reserve(kasid, seg, 0, REGION_OPT_GLOBAL | REGION_OPT_PRIVILEGED,
		     0, 3 * PAGESZ, &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  thread->machine.tss = (t_ia32_tss*)(t_vaddr)reg;

  memset(thread->machine.tss, 0x0, sizeof (t_ia32_tss));

  /* XXX one TSS per CPU */

  /*
   * 3)
   */

  if (segment_reserve(kasid, 2 * PAGESZ, PERM_READ | PERM_WRITE,
		      &seg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (region_reserve(kasid, seg, 0, REGION_OPT_GLOBAL | REGION_OPT_PRIVILEGED,
		     0, 2 * PAGESZ, &reg) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  int_stack = (t_vaddr)reg;

  /* XXX one int_stack per CPU */

  /*
   * 4)
   */

  if (ia32_tss_load(thread->machine.tss,
		    IA32_SEGSEL(IA32_PMODE_GDT_CORE_DS, IA32_PRIV_RING0),
		    int_stack + 2 * PAGESZ - 16,
		    0x68) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  ia32_cpu_local_set(&ia32_local_interrupt_stack, int_stack + 2 * PAGESZ - 16);

  /*
   * 5)
   */

  if (ia32_tss_init(thread->machine.tss) != STATUS_OK)	/* XXX per CPU */
    return (STATUS_UNKNOWN_ERROR);

  /*
   *
   */

  ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_CS, ia32_prvl_supervisor,
			  &thread->machine.core_cs);
  ia32_gdt_build_selector(IA32_PMODE_GDT_CORE_DS, ia32_prvl_supervisor,
			  &thread->machine.core_ds);
  ia32_gdt_build_selector(IA32_PMODE_GDT_DRIVER_CS, ia32_prvl_driver,
			  &thread->machine.driver_cs);
  ia32_gdt_build_selector(IA32_PMODE_GDT_DRIVER_DS, ia32_prvl_driver,
			  &thread->machine.driver_ds);
  ia32_gdt_build_selector(IA32_PMODE_GDT_SERVICE_CS, ia32_prvl_service,
			  &thread->machine.service_cs);
  ia32_gdt_build_selector(IA32_PMODE_GDT_SERVICE_DS, ia32_prvl_service,
			  &thread->machine.service_ds);
  ia32_gdt_build_selector(IA32_PMODE_GDT_PROGRAM_CS, ia32_prvl_user,
			  &thread->machine.program_cs);
  ia32_gdt_build_selector(IA32_PMODE_GDT_PROGRAM_DS, ia32_prvl_user,
			  &thread->machine.program_ds);

  return (STATUS_OK);
}

/*
 * this function store back a core thread stack pointer into the thread object.
 */

t_status		ia32_context_ring0_stack(void)
{
  i_thread		current;
  o_thread*		o;
  o_task*		otask;

  if (scheduler_current(&current) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (thread_get(current, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (task_get(o->taskid, &otask) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (otask->class == TASK_CLASS_KERNEL)
    {
      o->machine.interrupt_stack = ia32_cpu_local_get(ia32_local_jump_stack);
      o->machine.interrupt_stack += sizeof (t_ia32_context);
    }

  return (STATUS_OK);
}

/*
 * this function switches execution to the specified thread.
 *
 * steps:
 *
 * 1) check if a switch is necessary.
 * 2) get necessary objects.
 * 3) set jump stack & pdbr. update tss for next interrupt.
 * 4) update the I/O permissions bit map.
 * 5) set the TS flag so any use of FPU, MMX or SSE instruction will
 *    generate an exception.
 */

t_status		ia32_context_switch(i_thread		current,
					    i_thread		elected)
{
  o_thread*		from;
  o_thread*		to;
  o_task*		task;
  o_as*			as;
  t_vaddr		cr3;

  /*
   * 1)
   */

  if (current == elected)
      return (STATUS_OK);

  /*
   * 2)
   */

  if (thread_get(current, &from) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (thread_get(elected, &to) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (task_get(to->taskid, &task) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (as_get(task->asid, &as) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 3)
   */

  if (ia32_pd_get_cr3(&cr3,
		      as->machine.pd,
		      IA32_PD_CACHED,
		      IA32_PD_WRITEBACK) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  ia32_cpu_local_set(&ia32_local_jump_pdbr, cr3);

  ia32_cpu_local_set(&ia32_local_jump_stack, to->machine.interrupt_stack -
		     sizeof (t_ia32_context));

  if (ia32_tss_load(thread->machine.tss,	/* XXX per CPU */
		    IA32_SEGSEL(IA32_PMODE_GDT_CORE_DS, IA32_PRIV_RING0),
		    to->machine.interrupt_stack,
		    0x68) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 4)
   */

  if (1 || current == ID_UNUSED || /* XXX */
      (from->taskid != to->taskid && (task->class == TASK_CLASS_SERVICE ||
				      task->class == TASK_CLASS_GUEST)) ||
      task->machine.ioflush)
    {
      memcpy((t_uint8*)thread->machine.tss + thread->machine.tss->io,
	     &task->machine.iomap,
	     8192);
      task->machine.ioflush = 0;
    }

  /*
   * 5)
   */

  STS();

  return (STATUS_OK);
}

/*
 * this function is used to switch FPU, MMX and SSE context.
 *
 * steps:
 *
 * 1) clears the TS bit in CR0.
 * 2) get both executing and old contexts.
 * 3) SSE case
 *  a) save the SSE & FPU context into the previous thread context.
 *  b) restore the SSE & FPU context of the executing thread.
 * 4) FPU or MMX case
 *  a) save the FPU/MMX context into the previous thread context.
 *  b) restore the FPU/MMX context of the executing thread.
 */

t_status		ia32_extended_context_switch(i_thread	current,
						     i_thread	elected)
{
  o_thread*		o;
  o_thread*		old;

  /*
   * 1)
   */

  CLTS();

  if (current == elected)
    return (STATUS_OK);

  /*
   * 2)
   */

  if (thread_get(elected, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (thread_get(current, &old) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (ia32_cpucaps & IA32_CAPS_SSE)
    {
      /*
       * 3)
       */

      /*
       * a)
       */

      FXSAVE(old->machine.u.sse);

      /*
       * b)
       */

      FXRSTOR(o->machine.u.sse);
    }
  else
    {
      /*
       * 4)
       */

      /*
       * a)
       */

      FSAVE(old->machine.u.x87);

      /*
       * b)
       */

      FRSTOR(o->machine.u.x87);
    }

  return (STATUS_OK);
}

/*
 * this function pushes arguments on a thread's stack.
 *
 * steps:
 *
 * 1) get thread and task object.
 * 2) get current thread context and decrement the stack pointer.
 * 3) copy the argument onto the stack.
 * 4) update the thread context.
 */

t_status		ia32_push_args(i_thread			threadid,
				       const void*		args,
				       t_vsize			size)
{
  o_thread*		o;
  o_task*		otask;
  t_thread_context	context;

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (task_get(o->taskid, &otask) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  if (thread_store(threadid, &context) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  context.sp -= size;

  /*
   * 3)
   */

  if (as_write(otask->asid, args, size, context.sp) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 4)
   */

  if (thread_load(threadid, context) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  return (STATUS_OK);
}

/*
 * this function retrieves the context of a given thread.
 *
 * steps:
 *
 * 1) get thread & task objects.
 * 2) read the context.
 */

t_status		ia32_get_context(i_thread		thread,
					 t_ia32_context*	context)
{
  o_thread*		o;
  o_task*		otask;

  /*
   * 1)
   */

  if (thread_get(thread, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (task_get(o->taskid, &otask) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  if (as_read(otask->asid,
	      o->machine.interrupt_stack - sizeof (t_ia32_context),
	      sizeof (t_ia32_context),
	      context) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  return (STATUS_OK);
}

/*
 * this function updates the context of a thread given a set of register
 * values.
 *
 * steps:
 *
 * 1) get thread & task objects.
 * 2) read the current context.
 * 3) according to the mask, update the required registers.
 * 4) store back the context to the thread's stack.
 */

t_status		ia32_set_context(i_thread		thread,
					 t_ia32_context*	context,
					 t_uint32		mask)
{
  o_thread*		o;
  o_task*		otask;
  t_ia32_context	temp;

  /*
   * 1)
   */

  if (thread_get(thread, &o) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  if (task_get(o->taskid, &otask) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 2)
   */

  if (as_read(otask->asid,
	      o->machine.interrupt_stack - sizeof (t_ia32_context),
	      sizeof (t_ia32_context),
	      &temp) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  /*
   * 3)
   */

  if (mask & IA32_CONTEXT_EAX)
    temp.eax = context->eax;
  if (mask & IA32_CONTEXT_EBX)
    temp.ebx = context->ebx;
  if (mask & IA32_CONTEXT_ECX)
    temp.ecx = context->ecx;
  if (mask & IA32_CONTEXT_EDX)
    temp.edx = context->edx;
  if (mask & IA32_CONTEXT_ESI)
    temp.esi = context->esi;
  if (mask & IA32_CONTEXT_EDI)
    temp.edi = context->edi;
  if (mask & IA32_CONTEXT_EBP)
    temp.ebp = context->ebp;
  if (mask & IA32_CONTEXT_ESP)
    temp.esp = context->esp;
  if (mask & IA32_CONTEXT_EIP)
    temp.eip = context->eip;
  if (mask & IA32_CONTEXT_EFLAGS)
    temp.eflags = context->eflags;
  if (mask & IA32_CONTEXT_CS)
    temp.cs = context->cs;
  if (mask & IA32_CONTEXT_SS)
    temp.ss = context->ss;
  if (mask & IA32_CONTEXT_DS)
    temp.ds = context->ds;

  /*
   * 4)
   */

  if (as_write(otask->asid,
	       &temp, sizeof
	       (t_ia32_context),
	       o->machine.interrupt_stack - sizeof (t_ia32_context)) != STATUS_OK)
    return (STATUS_UNKNOWN_ERROR);

  return (STATUS_OK);
}
