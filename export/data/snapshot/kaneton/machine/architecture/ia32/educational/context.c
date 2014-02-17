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
 * updated       julien quintard   [mon feb  7 15:53:52 2011]
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

extern m_kernel		_kernel;

/*
 * thread manager.
 */

extern m_thread		_thread;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the given context.
 */

t_status			architecture_context_dump(as_context	context)
{
  module_call(console, message,
	      '#',
	      "context: ds(0x%x) edi(0x%x) esi(0x%x) ebp(0x%x) _esp(0x%x)\n",
	      context.ds & 0xffff,
	      context.edi,
	      context.esi,
	      context.ebp,
	      context._esp);

  module_call(console, message,
	      '#',
	      "         ebx(0x%x) edx(0x%x) ecx(0x%x) eax(0x%x) error(0x%x)\n",
	      context.ebx,
	      context.edx,
	      context.ecx,
	      context.eax,
	      context.error);

  module_call(console, message,
	      '#',
	      "         eip(0x%x) cs(0x%x) eflags(0x%x) esp(0x%x) ss(0x%x)\n",
	      context.eip,
	      context.cs & 0xffff,
	      context.eflags,
	      context.esp,
	      context.ss & 0xffff);

  MACHINE_LEAVE();
}

/*
 * this function builds the given context, initializes its attributes.
 *
 * steps:
 *
 * 1) retrieve the thread and task objects.
 * 2) depending on the thread's task class.
 *   A) if this thread is a kernel thread i.e a ring0 thread, this means
 *      that its context will be saved on its stack. besides, since no change
 *      in privilege will occur when interrupted, there is no need for a
 *       pile...
 *     a) set the pile attributes to zero.
 *   B) otherwise...
 *     a) set the thread's pile size.
 *     b) allocate a pile for the thread i.e a stack which is used by the
 *        processor to store the context whenever the execution privilege
 *        changes; for example whenever a guest task running in CP3 is
 *        interrupted by the timer.
 *     c) set the pile pointer to the end since IA32 stacks grow towards
 *        the lower addresses.
 * 3) initialize the IA32 context's registers to zero.
 * 4) initialize the eflags by activating the first bit (mandatory) but
 *    also the IF flags to that maskable interrupts get triggered. besides,
 *    allow driver tasks to perform I/O operations by setting the
 *    appropriate IOPL.
 * 5) set the context's segment selectors according to the task's class.
 * 6) complete the thread's initial IA32 context by setting ESP and EIP.
 * 7) set the static stack pointer to the end since stacks grow towards
 *    the lower addresses.
 * 8) depending on the thread's task class.
 *   A) if the thread is a kernel thread, set the initial context's position
 *      on the stack. indeed, let's recall that ring0 thread's contexts
 *      are stored on their stack since no change in privilege occurs.
 *   B) otherwise, set its position on the thread's pile i.e special kernel
 *      stack.
 * 9) finally set the thread's initial IA32 context. note that this step
 *    is ignored for the kernel thread. indeed, this thread is the one
 *    setting up the whole kernel. once the scheduler is started, an interrupt
 *    will be triggered hence interrupting the running thread i.e the kernel
 *    thread, hence saving its context. since this special thread will always
 *    start with its context being saved, there is no need to do it now.
 *    on the contrary, the other threads will begin with their context being
 *    restored in order to be scheduled for the first time.
 */

t_status			architecture_context_build(i_thread	id)
{
  o_task*		task;
  o_thread*		thread;
  as_context		ctx;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (task->class == TASK_CLASS_KERNEL)
    {
      /*
       * A)
       */

      /*
       * a)
       */

      thread->machine.pile.base = 0x0;
      thread->machine.pile.size = 0x0;
      thread->machine.pile.pointer = 0x0;
    }
  else
    {
      /*
       * B)
       */

      /*
       * a)
       */

      thread->machine.pile.size = ARCHITECTURE_HANDLER_PILE_SIZE;

      /*
       * b)
       */

      if (map_reserve(task->as,
		      MAP_OPTION_NONE,
		      thread->machine.pile.size,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &thread->machine.pile.base) != STATUS_OK)
	MACHINE_ESCAPE("unable to reserve a map for the thread's pile");

      /*
       * c)
       */

      thread->machine.pile.pointer =
	thread->machine.pile.base + thread->machine.pile.size - 16;
    }

  /*
   * 3)
   */

  memset(&ctx, 0x0, sizeof (as_context));

  /*
   * 4)
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
   * 5)
   */

  switch (task->class)
    {
      case TASK_CLASS_KERNEL:
	{
	  ctx.cs = _thread.machine.selectors.kernel.cs;
	  ctx.ds = _thread.machine.selectors.kernel.ds;
	  ctx.ss = _thread.machine.selectors.kernel.ds;

	  break;
	}
      case TASK_CLASS_DRIVER:
	{
	  ctx.cs = _thread.machine.selectors.driver.cs;
	  ctx.ds = _thread.machine.selectors.driver.ds;
	  ctx.ss = _thread.machine.selectors.driver.ds;

	  break;
	}
      case TASK_CLASS_SERVICE:
	{
	  ctx.cs = _thread.machine.selectors.service.cs;
	  ctx.ds = _thread.machine.selectors.service.ds;
	  ctx.ss = _thread.machine.selectors.service.ds;

	  break;
	}
      case TASK_CLASS_GUEST:
	{
	  ctx.cs = _thread.machine.selectors.guest.cs;
	  ctx.ds = _thread.machine.selectors.guest.ds;
	  ctx.ss = _thread.machine.selectors.guest.ds;

	  break;
	}
    }

  /*
   * 6)
   */

  ctx.esp = thread->stack.base + thread->stack.size - 16;
  ctx.eip = thread->entry;

  /*
   * 7)
   */

  thread->machine.stack.pointer = thread->stack.base + thread->stack.size - 16;

  /*
   * 8)
   */

  if (task->class == TASK_CLASS_KERNEL)
    {
      /*
       * A)
       */

      thread->machine.context =
	thread->machine.stack.pointer - sizeof (as_context);
    }
  else
    {
      /*
       * B)
       */

      thread->machine.context =
	thread->machine.pile.pointer - sizeof (as_context);
    }

  /*
   * 9)
   */

  if (thread->id != _kernel.thread)
    {
      if (architecture_context_set(thread->id, &ctx) != STATUS_OK)
	MACHINE_ESCAPE("unable to set the context");
    }

  MACHINE_LEAVE();
}

/*
 * this function destroys a thread's context.
 *
 * steps:
 *
 * 1) retrieve the thread and task object.
 * 2) if the thread has a pile---i.e is not a kernel thread---release it.
 */

t_status			architecture_context_destroy(i_thread	id)
{
  o_task*		task;
  o_thread*		thread;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (task->class != TASK_CLASS_KERNEL)
    {
      if (map_release(task->as,
		      thread->machine.pile.base) != STATUS_OK)
	MACHINE_ESCAPE("unable to release the thread's pile");
    }

  MACHINE_LEAVE();
}

/*
 * this function sets up the context switch mechanism.
 *
 * steps:
 *
 * 1) retrieve the kernel address space object.
 * 2) reserve the KIS - Kernel Interrupt Stack and set its pointer to
 *    the end since stacks grow towards low addresses. note that this
 *    special stack resides within the kernel, as its name indicates, and
 *    is used to handle interrupts.
 * 3) reserve a memory area for the TSS.
 * 4) build the initial TSS.
 * 5) update the TSS in order to represent the currently executing thread.
 *    since the current thread is the kernel thread, which runs in ring0,
 *    no ring0 stack needs to be provided, hence SS0 and ESP0 are ignored.
 * 6) activate the TSS.
 * 7) retrieve the segment selectors associated with the
 *    kernel/driver/service/guest code/data segments. these segment selectors
 *    will be used whenever a thread of the associated task class will
 *    be created.
 */

t_status			architecture_context_setup(void)
{
  as_tss*		tss;
  o_as*			as;

  /*
   * 1)
   */

  if (as_get(_kernel.as, &as) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the address space object");

  /*
   * 2)
   */

  _architecture.kernel.kis.size = ARCHITECTURE_HANDLER_KIS_SIZE;

  if (map_reserve(_kernel.as,
		  MAP_OPTION_SYSTEM,
		  _architecture.kernel.kis.size,
		  PERMISSION_READ | PERMISSION_WRITE,
		  &_architecture.kernel.kis.base) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve the TSS memory area");

  _architecture.kernel.kis.pointer =
    _architecture.kernel.kis.base + (_architecture.kernel.kis.size - 16);

  /*
   * 3)
   */

  if (map_reserve(_kernel.as,
		  MAP_OPTION_SYSTEM,
		  ARCHITECTURE_TSS_SIZE,
		  PERMISSION_READ | PERMISSION_WRITE,
		  &_thread.machine.tss) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve the TSS memory area");

  /*
   * 4)
   */

  if (architecture_tss_build(_thread.machine.tss, &tss) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the initial TSS");

  /*
   * 5)
   */

  if (architecture_tss_update(tss,
			      ARCHITECTURE_TSS_SS0_NULL,
			      ARCHITECTURE_TSS_ESP0_NULL,
			      ARCHITECTURE_TSS_IO) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the TSS");

  /*
   * 6)
   */

  if (architecture_tss_activate(tss) != STATUS_OK)
    MACHINE_ESCAPE("unable to activate the system's TSS");

  /*
   * 7)
   */

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_KERNEL_CODE,
	ARCHITECTURE_PRIVILEGE_KERNEL,
	&_thread.machine.selectors.kernel.cs) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the kernel code segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_KERNEL_DATA,
	ARCHITECTURE_PRIVILEGE_KERNEL,
	&_thread.machine.selectors.kernel.ds) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the kernel data segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_DRIVER_CODE,
	ARCHITECTURE_PRIVILEGE_DRIVER,
	&_thread.machine.selectors.driver.cs) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the driver code segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_DRIVER_DATA,
	ARCHITECTURE_PRIVILEGE_DRIVER,
	&_thread.machine.selectors.driver.ds) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the driver data segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_SERVICE_CODE,
	ARCHITECTURE_PRIVILEGE_SERVICE,
	&_thread.machine.selectors.service.cs) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the service code segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_SERVICE_DATA,
	ARCHITECTURE_PRIVILEGE_SERVICE,
	&_thread.machine.selectors.service.ds) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the service data segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_GUEST_CODE,
	ARCHITECTURE_PRIVILEGE_GUEST,
	&_thread.machine.selectors.guest.cs) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the guest code segment selector");

  if (architecture_gdt_selector(
        ARCHITECTURE_GDT_INDEX_GUEST_DATA,
	ARCHITECTURE_PRIVILEGE_GUEST,
	&_thread.machine.selectors.guest.ds) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the guest data segment selector");

  MACHINE_LEAVE();
}

/*
 * this function switches execution to the specified thread.
 */

t_status			architecture_context_switch(i_thread	current,
						    i_thread	future)
{
  /* FIXME[code to complete] */

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

t_status			architecture_context_arguments(i_thread	id,
						       void*	arguments,
						       t_vsize	size)
{
  s_thread_context	context;
  o_thread*		thread;
  o_task*		task;

  /*
   * 1)
   */

  if (thread_get(id, &thread) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (thread_store(thread->id, &context) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the thread context");

  /*
   * 3)
   */

  context.sp -= size;

  /*
   * 4)
   */

  if (as_write(task->as, arguments, size, context.sp) != STATUS_OK)
    MACHINE_ESCAPE("unable to store the arguments on the thread's stack");

  /*
   * 5)
   */

  if (thread_load(thread->id, context) != STATUS_OK)
    MACHINE_ESCAPE("unable to update the thread context");

  MACHINE_LEAVE();
}

/*
 * this function retrieves the IA32 context of the given thread.
 *
 * note that the interrupted task's context has been stored in its
 * pile, i.e ring0 stack (excepts for kernels threads). since the pile is
 * only mapped in the task's address space, this function, running in the
 * kernel environment, cannot access it directly. therefore, the as_read()
 * function is used to temporarily map the necessary pages in the kernel
 * address space.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the thread and task objects.
 * 2) read the thread's context from its pile.
 */

t_status			architecture_context_get(i_thread	id,
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

  if (thread_get(id, &thread) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (as_read(task->as,
	      thread->machine.context,
	      sizeof (as_context),
	      context) != STATUS_OK)
    MACHINE_ESCAPE("unable to read the thread's IA32 context");

  MACHINE_LEAVE();
}

/*
 * this function updates the context of a given thread.
 *
 * note that the interrupted task's context has been stored in its
 * pile, i.e ring0 stack (except for kernel threads). since the pile is
 * only mapped in the task's address space, this function, running in the
 * kernel environment, cannot access it directly. therefore, the as_write()
 * function is used to temporarily map the necessary pages in the kernel
 * address space.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) retrieve the thread and task objects.
 * 2) update the thread's context stored in its pile by writing its address
 *    space.
 */

t_status			architecture_context_set(i_thread	id,
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

  if (thread_get(id, &thread) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the thread object");

  if (task_get(thread->task, &task) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the task object");

  /*
   * 2)
   */

  if (as_write(task->as,
	       context,
	       sizeof (as_context),
	       thread->machine.context) != STATUS_OK)
    MACHINE_ESCAPE("unable to write the thread's IA32 context");

  MACHINE_LEAVE();
}
