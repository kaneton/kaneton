/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../architecture/ia32/educational/context.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       julien quintard   [wed jan 26 21:46:58 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functions related to the IA32 context management.
 *
 * the ia32/educational implementation makes use of a single TSS - Task
 * State Segment which describes the currently executing context.
 *
 * the context switch mechanism consists in saving the currently executing
 * thread's state and loading the future's. note however that the CPU
 * performs some saving/restoring automatically.
 *
 * basically, things go as follows. a task is running, say in CPL3 i.e a guest
 * task. when the timer interrupt occurs, for example, the privilege changes
 * from CPL3 to CPL0. the CPU, noticing this change in privilege saves some
 * of the thread's context---SS, ESP, EFLAGS, CS, EIP and possible an error
 * code---on a special stack referred to as the thread's pile i.e a stack
 * specifically used whenever the privilege changes. note that, should no
 * change in privilege occur, the registers would be stored on the thread's
 * current stack.
 *
 * at this point, the processor executes the handler shell (cf handler.c)
 * which pre-handles an interrupt depending on its nature: exception, IRQ etc.
 * besides, the handler shell calls the ARCHITECTURE_CONTEXT_SAVE()
 * macro-function which is at the heart of the context switching mechanism.
 *
 * once the interrupt has been treated, the ARCHITECTURE_CONTEXT_RESTORE()
 * macro-function restores the necessary. finally, the 'iret' instruction
 * is called. the CPU noticing that a privilege change had occured, restores
 * the thread's context by fetching the registers it pushed from the
 * thread's pile (or from the thread's stack if the privilege had not changed).
 *
 * the whole context switch mechanism therefore relies on interrupts. more
 * precisely the idea for context switching from a thread A to a thread B
 * is to (i) save the thread A's context but this is done naturally when
 * A gets interrupted and (ii) change the TSS so that thread B gets referenced,
 * hence making the CPU think B got interrupted (instead of A). therefore,
 * when returning from the interrupt, the CPU will restore B's context
 * rather than A's. this is how A got interrupted, its context saved
 * and B's execution got resumed.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the architecture manager.
 */

extern am		_architecture;

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
 * this function builds the given context, initializes its attributes.
 *
 * steps:
 *
 * 1) retrieve the thread and task objects.
 * 2) allocate a pile for the thread i.e a stack which is used by the
 *    processor to store the context whenever the execution privilege
 *    changes; for example whenever a guest task running in CP3 is
 *    interrupted by the timer.
 * 3) place the pile pointer at the end since IA32 stacks grow towards
 *    the lower addresses.
 * 4) initialize the IA32 context's registers to zero.
 * 5) initialize the eflags by activating the first bit (mandatory) but
 *    also the IF flags to that maskable interrupts get triggered. besides,
 *    allow driver tasks to perform I/O operations by setting the
 *    appropriate IOPL.
 * 6) set the context's segment selectors according to the task's class.
 * 7) finally, set the context.
 */

t_error			architecture_context_build(i_thread	id)
{
  o_task*		task;
  o_thread*		thread;
  as_context		ctx;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (map_reserve(task->as,
		  MAP_OPTION_NONE,
		  ARCHITECTURE_HANDLER_PILE_SIZE,
		  PERMISSION_READ | PERMISSION_WRITE,
		  &thread->machine.pile.base) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve a map for the thread's pile");

  /*
   * 3)
   */

  thread->machine.pile.esp =
    thread->machine.pile.base + (ARCHITECTURE_HANDLER_PILE_SIZE - 16);

  /*
   * 4)
   */

  memset(&ctx, 0, sizeof (as_context));

  /*
   * 5)
   */

  ctx.eflags =
    ARCHITECTURE_REGISTER_EFLAGS_01 |
    ARCHITECTURE_REGISTER_EFLAGS_IF;

  if (task->class == TASK_CLASS_DRIVER)
    {
      ctx.eflags |=
	ARCHITECTURE_REGISTER_EFLAGS_IOPL_SET(ARCHITECTURE_PRIVILEGE_DRIVER);
    }

  /*
   * 6)
   */

  switch (task->class)
    {
      case TASK_CLASS_KERNEL:
	{
	  ctx.cs = _thread->machine.selectors.kernel.cs;
	  ctx.ds = _thread->machine.selectors.kernel.ds;
	  ctx.ss = _thread->machine.selectors.kernel.ds;

	  break;
	}
      case TASK_CLASS_DRIVER:
	{
	  ctx.cs = _thread->machine.selectors.driver.cs;
	  ctx.ds = _thread->machine.selectors.driver.ds;
	  ctx.ss = _thread->machine.selectors.driver.ds;

	  break;
	}
      case TASK_CLASS_SERVICE:
	{
	  ctx.cs = _thread->machine.selectors.service.cs;
	  ctx.ds = _thread->machine.selectors.service.ds;
	  ctx.ss = _thread->machine.selectors.service.ds;

	  break;
	}
      case TASK_CLASS_GUEST:
	{
	  ctx.cs = _thread->machine.selectors.guest.cs;
	  ctx.ds = _thread->machine.selectors.guest.ds;
	  ctx.ss = _thread->machine.selectors.guest.ds;

	  break;
	}
    }

  /*
   * 7)
   */

  if (architecture_context_set(thread->id, &ctx) != ERROR_OK)
    MACHINE_ESCAPE("unable to set the context");

  MACHINE_LEAVE();
}

/*
 * this function sets up the context switch mechanism.
 *
 * steps:
 *
 * 1) retrieve the kernel address space object.
 * 2) reserve a segment for the TSS and map it.
 * 3) place the tss in the allocated region.
 * 4) initialize the TSS.
 * 5) reserve a segment for the KIS - Kernel Interrupt Stack i.e the stack
 *    used for treating interrupts, in the kernel environment. finally,
 *    map the segment.
 * 6) set up the KIS and initialize its stack pointer.
 * 7) build the segment selector associated with the kernel data segment.
 * 8) set up the TSS.
 * 9) activate the TSS.
 * 10) retrieve the segment selectors associated with the
 *     kernel/driver/service/guest code/data segments. these segment selectors
 *     will be used whenever a thread of the associated task class will
 *     be created.
 */

t_error			architecture_context_setup(void)
{
  t_uint16		selector;
  i_segment		segment;
  i_region		region;
  as_tss*		tss;
  o_as*			as;
  o_region*		o;

  /*
   * 1)
   */

  if (as_get(_kernel->as, &as) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  if (segment_reserve(_kernel->as,
		      ARCHITECTURE_TSS_SIZE,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_SYSTEM,
		      &segment) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve a segment");

  if (region_reserve(_kernel->as,
		     segment,
		     0x0,
		     REGION_OPTION_NONE,
		     0x0,
		     ARCHITECTURE_TSS_SIZE,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve a region");

  if (region_get(_kernel->as, region, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 3)
   */

  _thread->machine.tss = o->address;

  /*
   * 4)
   */

  tss = (as_tss*)_thread->machine.tss;

  memset(tss, 0x0, sizeof (as_tss));

  /*
   * 5)
   */

  if (segment_reserve(_kernel->as,
		      ARCHITECTURE_HANDLER_KIS_SIZE,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_SYSTEM,
		      &segment) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve a segment");

  if (region_reserve(_kernel->as,
		     segment,
		     0x0,
		     REGION_OPTION_NONE,
		     0x0,
		     ARCHITECTURE_HANDLER_KIS_SIZE,
		     &region) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve a region");

  if (region_get(_kernel->as, region, &o) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 6)
   */

  _architecture.kernel.kis.base = o->address;
  _architecture.kernel.kis.esp =
    _architecture.kernel.kis.base + (ARCHITECTURE_HANDLER_KIS_SIZE - 16);

  /*
   * 7)
   */

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_DATA,
				ARCHITECTURE_PRIVILEGE_RING0,
				&selector) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the kernel data segment selector");

  /*
   * 8)
   */

  if (architecture_tss_update(tss,
			      selector,
			      _architecture.kernel.kis.esp,
			      // XXX ici ca devrait etre la pile du thread kernel. si on change cette valeur alors ESP dans SAVE() sera different
			      ARCHITECTURE_TSS_IO_OFFSET) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the TSS");

  /*
   * 9)
   */

  if (architecture_tss_activate(tss) != ERROR_OK)
    MACHINE_ESCAPE("unable to activate the system's TSS");

  /*
   * 10)
   */

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_KERNEL_CODE,
	ARCHITECTURE_PRIVILEGE_KERNEL,
	&_thread->machine.selectors.kernel.cs) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the kernel code segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_KERNEL_DATA,
	ARCHITECTURE_PRIVILEGE_KERNEL,
	&_thread->machine.selectors.kernel.ds) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the kernel data segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_DRIVER_CODE,
	ARCHITECTURE_PRIVILEGE_DRIVER,
	&_thread->machine.selectors.driver.cs) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the driver code segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_DRIVER_DATA,
	ARCHITECTURE_PRIVILEGE_DRIVER,
	&_thread->machine.selectors.driver.ds) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the driver data segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_SERVICE_CODE,
	ARCHITECTURE_PRIVILEGE_SERVICE,
	&_thread->machine.selectors.service.cs) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the service code segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_SERVICE_DATA,
	ARCHITECTURE_PRIVILEGE_SERVICE,
	&_thread->machine.selectors.service.ds) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the service data segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_GUEST_CODE,
	ARCHITECTURE_PRIVILEGE_GUEST,
	&_thread->machine.selectors.guest.cs) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the guest code segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_GUEST_DATA,
	ARCHITECTURE_PRIVILEGE_GUEST,
	&_thread->machine.selectors.guest.ds) != ERROR_OK)
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

  if (thread_current(&current) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (thread_get(current, &othread) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (task_get(othread->task, &otask) != ERROR_OK)
    MACHINE_ESCAPE("XXX");

  if (otask->class == TASK_CLASS_KERNEL)
    {
      // XXX le thread kernel est interrompu -> il n'y a pas changement
      // de privilege donc on n'utilise pas sa pile (qui ne sera donc
      // jamais utilisee) -> CONTEXT_SAVE() va donc prendre l'ESP courant
      // et le considere comme etant la pile donc le sauver dans
      // architecture.thread.pile.esp alors qu'en fait c'est juste sa stack.
      //
      // XXX lors d'un context switch vers le kernel thread, on va prendre
      // la pile du thread (kernel) et lors du CONTEXT_RESTORE() on va chercher
      // le context du thread sur cette pile. malheureusement le contexte
      // du thread kernel a ete sauve sur la stack et non sur la pile.
      //
      // XXX pour cette raison, on hack le truc ici on mettant a jour
      // tout thread qui tourne en ring0 (kernel threads) pour que la pile
      // coincide avec la stack et que le context soit bien retrouve.
      //
      // XXX un bien meilleure solution serait, dans le build, de ne pas
      // creer de pile si class == kernel et de faire pointer la pile sur
      // la stack.
      //
      // XXX un autre solution serait de ne passer (dans restore) sur
      // la pile que si le thread a restore n'est pas deja en ring0.
      othread->machine.pile.esp = _architecture.thread.pile.esp;

      // XXX et ca c'est surement un hack car dans context switch on
      // va faire un -= sizeof(...) donc on contrecarre ici.
      othread->machine.pile.esp += sizeof (as_context);
    }

  MACHINE_LEAVE();
}

/*
 * this function switches execution to the specified thread.
 *
 * steps:
 *
 * 1) if the future thread is the current one, return as no context
 *    has to be switched.
 * 2) retrieve the system's TSS.
 * 3) retrieve the current and future thread objects.
 * 4) retrieve the future task object.
 * 5) retrieve the fhture address space object.
 * 6) build the PDBR - Page Directory Base Register associated with the
 *    future address space.
 * 7) set, in the architecture's structure the about-to-be current PDBR
 *    as being the future's one. since the system is going to return from
 *    the interrupt, the ARCHITECTURE_CONTEXT_RESTORE() will use the
 *    PDBR as the one to restore.
 * 8) set, in the architecture's structure, the future thread's pile pointer
 *    as being the one to restore. once again, ARCHITECTURE_CONTEXT_RESTORE()
 *    will use this value to restore the pile. note that since the CPU
 *    had saved the thread's context on the pile the ESP is computed
 *    accordingly.
 * 9) build the kernel data segment selector.
 * 10) update the TSS, only updating the pointer to the pile of the thread
 *     to restore. since this pile contains the saved context, the CPU
 *     can retrieve the necessary information for restoring the thread's
 *     state and resuming its execution.
 * 11) if the future thread belongs to another task and has no right to
 *     perform I/O operation (service or guest tasks) or that the task
 *     I/O map has been marked as dirty, refresh the TSS's I/O map.
 *   a) copy the task's I/O map.
 *   b) mark it as fresh.
 */

t_error			architecture_context_switch(i_thread	current,
						    i_thread	future)
{
  o_thread*		from;
  o_thread*		to;
  o_task*		task;
  o_as*			as;
  at_cr3		pdbr;
  as_tss*		tss;
  t_uint16		selector;

  /*
   * 1)
   */

  if (current == future)
    MACHINE_LEAVE();

  /*
   * 2)
   */

  tss = (as_tss*)_thread->machine.tss;

  /*
   * 3)
   */

  if (thread_get(current, &from) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the current thread object");

  if (thread_get(future, &to) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the future thread object");

  /*
   * 4)
   */

  if (task_get(to->task, &task) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the future task object");

  /*
   * 5)
   */

  if (as_get(task->as, &as) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the future address space object");

  /*
   * 6)
   */

  if (architecture_paging_pdbr(as->machine.pd,
			       ARCHITECTURE_REGISTER_CR3_PCE |
			       ARCHITECTURE_REGISTER_CR3_PWB,
			       &pdbr) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the PDBR");

  /*
   * 7)
   */

  _architecture.thread.pdbr = pdbr;

  /*
   * 8)
   */

  _architecture.thread.pile.esp = to->machine.pile.esp - sizeof (as_context);

  /*
   * 9)
   */

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_DATA,
				ARCHITECTURE_PRIVILEGE_RING0,
				&selector) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the kernel data segment selector");

  /*
   * 10)
   */

  if (architecture_tss_update(tss,
			      selector,
			      to->machine.pile.esp,
			      ARCHITECTURE_TSS_IO_OFFSET) != ERROR_OK)
    MACHINE_ESCAPE("unable to build the TSS");

  /*
   * 11)
   */

  if (((from->task != to->task) &&
       ((task->class == TASK_CLASS_SERVICE) ||
	(task->class == TASK_CLASS_GUEST))) ||
      (task->machine.io.flush == BOOLEAN_TRUE))
    {
      /*
       * a)
       */

      memcpy((t_uint8*)tss + tss->io,
	     &task->machine.io.map,
	     sizeof (task->machine.io.map));

      /*
       * b)
       */

      task->machine.io.flush = BOOLEAN_FALSE;
    }

  MACHINE_LEAVE();
}

/*
 * this function pushes the given arguments on a thread's stack. this way,
 * the thread will be able to access these values at startup.
 *
 * steps:
 *
 * 1) retrieve the thread and task objects.
 * 2) retrieve the thread's context.
 * 3) update the thread's stack pointer by decreasing it since the
 *    arguments are going to be stored at the top of it.
 * 4) write the arguments to the thread's stack.
 * 5) update the thread's context.
 */

t_error			architecture_context_arguments(i_thread	id,
						       void*	arguments,
						       t_vsize	size)
{
  s_thread_context	context;
  o_thread*		thread;
  o_task*		task;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (thread_store(thread->id, &context) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread context");

  /*
   * 3)
   */

  context.sp -= size;

  /*
   * 4)
   */

  if (as_write(task->as, arguments, size, context.sp) != ERROR_OK)
    MACHINE_ESCAPE("unable to store the arguments on the thread's stack");

  /*
   * 5)
   */

  if (thread_load(thread->id, context) != ERROR_OK)
    MACHINE_ESCAPE("unable to update the thread context");

  MACHINE_LEAVE();
}

/*
 * this function retrieves the IA32 context of the given thread.
 *
 * note that the interrupted task's context has been stored in its
 * pile, i.e ring0 stack. since the pile is only mapped in the task's
 * address space, this function, running in the kernel environment, cannot
 * access it directly. therefore, the task's pile is temporarily mapped
 * in the kernel address space the time for the context to be copied.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the thread and task objects.
 * 2) read the thread's context from its pile.
 */

t_error			architecture_context_get(i_thread	id,
						 as_context*	context)
{
  o_thread*		thread;
  o_task*		task;

  /*
   * 0)
   */

  if (context == NULL)
    MACHINE_ESCAPE("the 'context' argument is null");

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (as_read(task->as,
	      thread->machine.pile.esp - sizeof (as_context),
	      sizeof (as_context),
	      context) != ERROR_OK)
    MACHINE_ESCAPE("unable to extract the thread's IA32 context "
		   "from its pile");

  MACHINE_LEAVE();
}

/*
 * this function updates the context of a given thread.
 *
 * note that the interrupted task's context has been stored in its
 * pile, i.e ring0 stack. since the pile is only mapped in the task's
 * address space, this function, running in the kernel environment, cannot
 * access it directly. therefore, the task's pile is temporarily mapped
 * in the kernel address space the time for the context to be updated.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the thread and task objects.
 * 2) update the thread's context stored in its pile by writing its address
 *    space.
 */

t_error			architecture_context_set(i_thread	id,
						 as_context*	context)
{
  o_thread*		thread;
  o_task*		task;

  /*
   * 0)
   */

  if (context == NULL)
    MACHINE_ESCAPE("the 'context' argument is null");

  /*
   * 1)
   */

  if (thread_get(id, &thread) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != ERROR_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (as_write(task->as,
	       context,
	       sizeof (as_context),
	       thread->machine.pile.esp - sizeof (as_context)) != ERROR_OK)
    MACHINE_ESCAPE("unable to write the thread's IA32 context in "
		   "its pile");

  MACHINE_LEAVE();
}
