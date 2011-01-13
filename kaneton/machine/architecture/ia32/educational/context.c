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
 * stack switching addresses
 */

// XXX stack dans le kernel qui est utilisee pour traiter une interrupt
ARCHITECTURE_LINKER_LOCATION(".handler_data")
t_uint32	ia32_local_interrupt_stack = 0;

// XXX pour le context switch: la stack kernel du thread a sched + son PDBR
// XXX la thread's pile est utilisee lors d'une int/exc pour sauvegarder
//  le contexte. a ne pas confondre avec ia32_local_interrupt_stack qui
//  est la stack utilisee pour traiter une interrupt par le kernel.
ARCHITECTURE_LINKER_LOCATION(".handler_data")
t_uint32	ia32_local_jump_stack = 0;

ARCHITECTURE_LINKER_LOCATION(".handler_data")
t_uint32	ia32_local_jump_pdbr = 0;

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * kernel manager.
 */

extern m_kernel*	_kernel;

/*
 * thread manager.
 */

extern m_thread*	_thread;

/*
 * ---------- functions -------------------------------------------------------
 */

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

t_error			ia32_init_context(i_task		taskid,
					  i_thread		threadid)
{
  o_thread*		o;
  o_task*		task;
  t_ia32_context	ctx;


  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 2)
   */

  if (task_get(taskid, &task) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 3)
   */

  if (map_reserve(task->as,
		  MAP_OPTION_NONE,
		  ___kaneton$pagesz,
		  PERMISSION_READ | PERMISSION_WRITE,
		  &o->machine.pile) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  //printf("INTERRUPT STACK: 0x%x\n", o->machine.pile);

  o->machine.pile += (___kaneton$pagesz - 16);

  /*
   * 4)
   */

  memset(&ctx, 0, sizeof (t_ia32_context));

  // XXX 9 = Interrupt Enabled Flags
  ctx.eflags = (1 << 9) | (1 << 1);

  // XXX 12 = IO Privilege Level = 1
  // XXX => drivers can access I/O ports withouth the CPU checking the
  //  IO map
  if (task->class == TASK_CLASS_DRIVER)
    ctx.eflags |= (1 << 12);

  switch (task->class)
    {
      case TASK_CLASS_KERNEL:
	ctx.ds = ctx.ss = _thread->machine.selectors.kernel.ds;
	ctx.cs = _thread->machine.selectors.kernel.cs;
	break;
      case TASK_CLASS_DRIVER:
	ctx.ds = ctx.ss = _thread->machine.selectors.driver.ds;
	ctx.cs = _thread->machine.selectors.driver.cs;
	break;
      case TASK_CLASS_SERVICE:
	ctx.ds = ctx.ss = _thread->machine.selectors.service.ds;
	ctx.cs = _thread->machine.selectors.service.cs;
	break;
      case TASK_CLASS_GUEST:
	ctx.ds = ctx.ss = _thread->machine.selectors.guest.ds;
	ctx.cs = _thread->machine.selectors.guest.cs;
	break;
    }

  if (ia32_set_context(threadid, &ctx, IA32_CONTEXT_FULL) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  MACHINE_LEAVE();
}


/*
 * this function duplicate the whole context of a thread.
 */

t_error			ia32_duplicate_context(i_thread		old,
					       i_thread		new)
{
  o_thread*		from;
  o_thread*		to;
  t_ia32_context	ctx;

  if (thread_get(old, &from) != ERROR_OK ||
      thread_get(new, &to) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (ia32_get_context(old, &ctx) != ERROR_OK ||
      ia32_set_context(new, &ctx, IA32_CONTEXT_FULL) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  MACHINE_LEAVE();
}

/*
 * this function updates the context with the new stack and
 * instruction pointers.
 */

t_error			ia32_setup_context(i_thread		threadid,
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

t_error			ia32_status_context(i_thread		threadid,
					    t_vaddr*		pc,
					    t_vaddr*		sp)
{
  t_ia32_context	ctx;

  assert(pc != NULL);
  assert(sp != NULL);

  if (ia32_get_context(threadid, &ctx) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  *pc = ctx.eip;
  *sp = ctx.esp;

  MACHINE_LEAVE();
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
  i_segment		seg;
  i_region		reg;
  t_vaddr		int_stack;
  o_region*		r;
  as_tss*		tss;
  t_uint16		selector;

  /*
   * 1)
   */

  if (as_get(_kernel->as, &as) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 2)
   */

  // XXX a-t-on vraiment besoin de 3 pages pour le TSS?
  // XXX allouer que ce qu'il faut!
  if (segment_reserve(_kernel->as,
		      3 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (region_reserve(_kernel->as,
		     seg,
		     0,
		     REGION_OPTION_GLOBAL | REGION_OPTION_PRIVILEGED,
		     0,
		     3 * ___kaneton$pagesz,
		     &reg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (region_get(_kernel->as, reg, &r) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  _thread->machine.tss = r->address;

  tss = (as_tss*)_thread->machine.tss;

  //printf("TSS 0x%x\n", _thread->machine.tss);

  memset(tss, 0x0, sizeof(as_tss));

  /*
   * 3)
   */

  // XXX pourquoi allouer 2 pages?
  if (segment_reserve(_kernel->as,
		      2 * ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &seg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (segment_type(seg, SEGMENT_TYPE_SYSTEM) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (region_reserve(_kernel->as,
		     seg,
		     0,
		     REGION_OPTION_GLOBAL | REGION_OPTION_PRIVILEGED,
		     0,
		     2 * ___kaneton$pagesz,
		     &reg) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (region_get(_kernel->as, reg, &r) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  // XXX int_stack c'est donc quoi? la stack kernel globale?
  int_stack = r->address;

  //printf("INT STACK 0x%x\n", int_stack);

  /*
   * 4)
   */

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_DATA,
				ARCHITECTURE_PRIVILEGE_RING0,
				&selector) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the kernel data segment selector");

  if (architecture_tss_update(tss,
			      selector,
			      int_stack + 2 * ___kaneton$pagesz - 16,
			      TSS_IO_OFFSET) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the TSS");

  ia32_local_interrupt_stack = int_stack + 2 * ___kaneton$pagesz - 16;

  /*
   * 5)
   */

  if (architecture_tss_activate(tss) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * XXX
   */

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_CODE,
				ARCHITECTURE_PRIVILEGE_KERNEL,
				&_thread->machine.selectors.kernel.cs) !=
      ERROR_OK)
    MACHINE_ESCAPE("unable to build the kernel code segment selector");

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_DATA,
				ARCHITECTURE_PRIVILEGE_KERNEL,
				&_thread->machine.selectors.kernel.ds) !=
      ERROR_OK)
    MACHINE_ESCAPE("unable to build the kernel data segment selector");

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_DRIVER_CODE,
				ARCHITECTURE_PRIVILEGE_DRIVER,
				&_thread->machine.selectors.driver.cs) !=
      ERROR_OK)
    MACHINE_ESCAPE("unable to build the driver code segment selector");

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_DRIVER_DATA,
				ARCHITECTURE_PRIVILEGE_DRIVER,
				&_thread->machine.selectors.driver.ds) !=
      ERROR_OK)
    MACHINE_ESCAPE("unable to build the driver data segment selector");

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_SERVICE_CODE,
				ARCHITECTURE_PRIVILEGE_SERVICE,
				&_thread->machine.selectors.service.cs) !=
      ERROR_OK)
    MACHINE_ESCAPE("unable to build the service code segment selector");

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_SERVICE_DATA,
				ARCHITECTURE_PRIVILEGE_SERVICE,
				&_thread->machine.selectors.service.ds) !=
      ERROR_OK)
    MACHINE_ESCAPE("unable to build the service data segment selector");

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_GUEST_CODE,
				ARCHITECTURE_PRIVILEGE_GUEST,
				&_thread->machine.selectors.guest.cs) !=
      ERROR_OK)
    MACHINE_ESCAPE("unable to build the guest code segment selector");

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_GUEST_DATA,
				ARCHITECTURE_PRIVILEGE_GUEST,
				&_thread->machine.selectors.guest.ds) !=
      ERROR_OK)
    MACHINE_ESCAPE("unable to build the guest data segment selector");

  MACHINE_LEAVE();
}

/*
 * this function store back a core thread stack pointer into the thread object.
 */

t_error			ia32_context_ring0_stack(void)
{
  i_thread		current;
  o_thread*		othread;
  o_task*		otask;

  //printf("[XXX] ia32_context_ring0_stack()\n");

  if (thread_current(&current) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (thread_get(current, &othread) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (task_get(othread->task, &otask) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (otask->class == TASK_CLASS_KERNEL)
    {
      othread->machine.pile = ia32_local_jump_stack;

      othread->machine.pile +=
	sizeof (t_ia32_context);
    }

  //printf("[XXX] /ia32_context_ring0_stack()\n");

  MACHINE_LEAVE();
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

t_error			ia32_context_switch(i_thread		current,
					    i_thread		elected)
{
  o_thread*		from;
  o_thread*		to;
  o_task*		task;
  o_as*			as;
  t_vaddr		cr3;
  as_tss*		tss;
  t_uint16		selector;

  /*
   * 1)
   */

  if (current == elected)
    MACHINE_LEAVE();

  //printf("[context switch] from %qd to %qd\n", current, elected);

  tss = (as_tss*)_thread->machine.tss;

  /*
   * 2)
   */

  if (thread_get(current, &from) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (thread_get(elected, &to) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (task_get(to->task, &task) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (as_get(task->as, &as) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 3)
   */

  /* XXX[old]
  if (ia32_pd_get_cr3(&cr3,
		      (t_ia32_directory)as->machine.pd,
		      IA32_PAGE_DIRECTORY_CACHED,
		      IA32_PAGE_DIRECTORY_WRITEBACK) != ERROR_OK)
    MACHINE_ESCAPE("XXX");
  */
  if (architecture_paging_cr3((at_pd)as->machine.pd, // XXX
			      ARCHITECTURE_REGISTER_CR3_PCE |
			      ARCHITECTURE_REGISTER_CR3_PWB,
			      &cr3) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the CR3 register's content");

  /* XXX
  printf("CONTEXT SWITCH: [cr3] 0x%x 0x%x\n",
	 ia32_local_jump_pdbr,
	 cr3);
  */

  ia32_local_jump_pdbr = cr3;

  /* XXX
  printf("CONTEXT SWITCH: [interrupt stack] 0x%x 0x%x\n",
	 ia32_local_jump_stack,
	 to->machine.pile - sizeof (t_ia32_context));
  */

  ia32_local_jump_stack = to->machine.pile - sizeof (t_ia32_context);

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_DATA,
				ARCHITECTURE_PRIVILEGE_RING0,
				&selector) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the kernel data segment selector");

  if (architecture_tss_update(tss,
			      selector,
			      to->machine.pile,
			      TSS_IO_OFFSET) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the TSS");

  /*
   * 4)
   */

  if (current == ID_UNUSED || // XXX impossible kernel ou idle ou autre
      (from->task != to->task && (task->class == TASK_CLASS_SERVICE ||
				  task->class == TASK_CLASS_GUEST)) ||
      task->machine.io.flush == BOOLEAN_TRUE)
    {
      memcpy((t_uint8*)tss + tss->io,
	     &task->machine.io.map,
	     8192); // XXX sizeof
      task->machine.io.flush = BOOLEAN_FALSE;
    }

  /*
   * 5)
   */

  // XXX inutile, juste pour activer x87
  //STS();

  MACHINE_LEAVE();
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

t_error			ia32_push_args(i_thread			threadid,
				       const void*		args,
				       t_vsize			size)
{
  o_thread*		o;
  o_task*		otask;
  s_thread_context	context;

  /*
   * 1)
   */

  if (thread_get(threadid, &o) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (task_get(o->task, &otask) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 2)
   */

  if (thread_store(threadid, &context) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  context.sp -= size;

  /*
   * 3)
   */

  if (as_write(otask->as, args, size, context.sp) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 4)
   */

  if (thread_load(threadid, context) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  MACHINE_LEAVE();
}

/*
 * this function retrieves the context of a given thread.
 *
 * XXX the context is stored in the thread's kernel stack i.e pile.
 * since this stack is only mapped in the kernel's address space, we
 * cannot access it directly. therefore, an as_read() is used.

 * steps:
 *
 * 1) get thread & task objects.
 * 2) read the context.
 */

t_error			ia32_get_context(i_thread		thread,
					 t_ia32_context*	context)
{
  o_thread*		o;
  o_task*		otask;

  //printf("[XXX] ia32_get_context(%qd, 0x%x)\n", thread, context);

  /*
   * 1)
   */

  if (thread_get(thread, &o) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (task_get(o->task, &otask) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 2)
   */

  if (as_read(otask->as,
	      o->machine.pile - sizeof (t_ia32_context),
	      sizeof (t_ia32_context),
	      context) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  //printf("[XXX] /ia32_get_context(%qd, 0x%x)\n", thread, context);

  MACHINE_LEAVE();
}

/*
 * XXX
 */
t_error                 ia32_print_context(i_thread             thread)
{
    t_ia32_context      ctxt;

    ia32_get_context(thread, &ctxt);

    module_call(console, print,
		"Context:\n"
		"eax: 0x%08x   ebx: 0x%08x   ecx: 0x%08x\n"
		"edx: 0x%08x   esi: 0x%08x   edi: 0x%08x\n"
		"ebp: 0x%08x   esp: 0x%08x   eip: 0x%08x\n",
		ctxt.eax, ctxt.ebx, ctxt.ecx,
		ctxt.edx, ctxt.esi, ctxt.edi,
		ctxt.ebp, ctxt.orig_esp, ctxt.eip);

    MACHINE_LEAVE();
}

/*
 * this function updates the context of a thread given a set of register
 * values.
 *
 * XXX does the opposite of get_context()
 *
 * steps:
 *
 * 1) get thread & task objects.
 * 2) read the current context.
 * 3) according to the mask, update the required registers.
 * 4) store back the context to the thread's stack.
 */

t_error			ia32_set_context(i_thread		thread,
					 t_ia32_context*	context,
					 t_uint32		mask)
{
  o_thread*		o;
  o_task*		otask;
  t_ia32_context	temp;

  /*
   * 1)
   */

  if (thread_get(thread, &o) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (task_get(o->task, &otask) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  /*
   * 2)
   */

  if (as_read(otask->as,
	      o->machine.pile - sizeof (t_ia32_context),
	      sizeof (t_ia32_context),
	      &temp) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

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

  if (as_write(otask->as,
	       &temp, sizeof
	       (t_ia32_context),
	       o->machine.pile -
	         sizeof (t_ia32_context)) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  MACHINE_LEAVE();
}
