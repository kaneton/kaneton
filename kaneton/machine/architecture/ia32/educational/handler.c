/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../architecture/ia32/educational/handler.c
 *
 * created       renaud voltz   [thu feb 23 10:49:43 2006]
 * updated       julien quintard   [mon feb  7 11:04:14 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements events handling being for exceptions, IRQs, etc.
 *
 * note that every handler is called by its shell. a shell is a generated
 * piece a assembly code which sets up the environment so that the
 * handler can be written in C without worrying about the restoring the
 * stack properly depending on the nature of the interrupt/exception etc.
 *
 * whenever a non-ring0 task, executing and storing data on its stack
 * (thread->stack) is interrupted by an internal or external event, the
 * privilege changes from ring3 for example, to ring0. since the privilege
 * has changed, this microprocessor saves the interrupted thread's state onto
 * its ring0 stack i.e its pile (thread->machine.pile).
 *
 * at this point, the interrupt shell is triggered. this code lives in a
 * small area of memory along its data (within sections .handler_code and
 * .handler_data) and is mapped in every address space.
 *
 * the interrupt shells basically consists in saving and restoring the
 * interrupt thread's context. this mechanism is however taken care of by
 * the macro-functions ARCHITECTURE_CONTEXT_SAVE() and
 * ARCHITECTURE_CONTEXT_RESTORE().
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the kernel manager.
 */

extern m_kernel		_kernel;

/*
 * the architecture manager.
 */

extern am		_architecture;

/*
 * ---------- prototypes ------------------------------------------------------
 */

/*
 * this macro-function call generates the prehandler prototypes.
 */

ARCHITECTURE_HANDLER_SHELL_PROTOTYPES();

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * this table contains the handler shell associated with a given IDT entry
 * number starting with zero to the last syscall entry in use.
 */

at_handler_shell _architecture_handler_shells[ARCHITECTURE_HANDLER_SIZE] =
  {
    /*
     * exceptions
     */
    architecture_handler_shell_exception0,
    architecture_handler_shell_exception1,
    architecture_handler_shell_exception2,
    architecture_handler_shell_exception3,
    architecture_handler_shell_exception4,
    architecture_handler_shell_exception5,
    architecture_handler_shell_exception6,
    architecture_handler_shell_exception7,
    architecture_handler_shell_exception8,
    architecture_handler_shell_exception9,
    architecture_handler_shell_exception10,
    architecture_handler_shell_exception11,
    architecture_handler_shell_exception12,
    architecture_handler_shell_exception13,
    architecture_handler_shell_exception14,
    architecture_handler_shell_exception15,
    architecture_handler_shell_exception16,
    architecture_handler_shell_exception17,
    architecture_handler_shell_exception18,
    architecture_handler_shell_exception19,
    architecture_handler_shell_exception20,
    architecture_handler_shell_exception21,
    architecture_handler_shell_exception22,
    architecture_handler_shell_exception23,
    architecture_handler_shell_exception24,
    architecture_handler_shell_exception25,
    architecture_handler_shell_exception26,
    architecture_handler_shell_exception27,
    architecture_handler_shell_exception28,
    architecture_handler_shell_exception29,
    architecture_handler_shell_exception30,
    architecture_handler_shell_exception31,

    /*
     * IRQs - Interrupt Requests
     */
    architecture_handler_shell_irq0,
    architecture_handler_shell_irq1,
    architecture_handler_shell_irq2,
    architecture_handler_shell_irq3,
    architecture_handler_shell_irq4,
    architecture_handler_shell_irq5,
    architecture_handler_shell_irq6,
    architecture_handler_shell_irq7,
    architecture_handler_shell_irq8,
    architecture_handler_shell_irq9,
    architecture_handler_shell_irq10,
    architecture_handler_shell_irq11,
    architecture_handler_shell_irq12,
    architecture_handler_shell_irq13,
    architecture_handler_shell_irq14,
    architecture_handler_shell_irq15,

    /*
     * IPI - Inter-Processor Interrupts
     *
     * note that these are ignored because the educational ia32
     * implementation does not support SMP systems.
     */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    /*
     * syscalls.
     *
     * note that only ten syscalls are supported though this number
     * could be further extended if necessary later.
     */
    architecture_handler_shell_syscall0,
    architecture_handler_shell_syscall1,
    architecture_handler_shell_syscall2,
    architecture_handler_shell_syscall3,
    architecture_handler_shell_syscall4,
    architecture_handler_shell_syscall5,
    architecture_handler_shell_syscall6,
    architecture_handler_shell_syscall7,
    architecture_handler_shell_syscall8,
    architecture_handler_shell_syscall9 
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function sets up the hardware event handlers so as to be possibly
 * processed by a core handler.
 *
 * note that the IPI - Inter-Processor Interrupts are ignored since not
 * supported by this architecture.
 *
 * steps:
 *
 * 1) reserve a system segment and map it.
 * 2) build an IDT in this segment and set it up as being the current one.
 * 3) build the kernel code selector since required for registering the
 *    IRQs, exceptions etc. handlers in the IDT.
 * 4) register the exception shells hence handlers.
 * 5) register the IRQ shells hence handlers.
 * 6) register the syscall shells hence handlers.
 */

t_status		architecture_handler_setup(void)
{
  t_uint16		selector;
  i_segment		segment;
  i_region		region;
  as_idt		idt;
  o_region*		o;
  t_uint32		i;

  /*
   * 1)
   */

  if (segment_reserve(_kernel.as,
		      ___kaneton$pagesz,
		      PERMISSION_READ | PERMISSION_WRITE,
		      SEGMENT_OPTION_SYSTEM,
		      &segment) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve a segment");

  if (region_reserve(_kernel.as,
		     segment,
		     0x0,
		     REGION_OPTION_NONE,
		     0x0,
		     ___kaneton$pagesz,
		     &region) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve a region for the segment");

  if (region_get(_kernel.as, region, &o) != STATUS_OK)
    MACHINE_ESCAPE("unable to retrieve the region object");

  /*
   * 2)
   */

  if (architecture_idt_build(o->address,
			     ARCHITECTURE_IDT_SIZE,
			     &idt) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the IDT at the given address");

  if (architecture_idt_import(&idt) != STATUS_OK)
    MACHINE_ESCAPE("unable to import the built IDT");

  /*
   * 3)
   */

  if (architecture_gdt_selector(ARCHITECTURE_GDT_INDEX_KERNEL_CODE,
				ARCHITECTURE_PRIVILEGE_KERNEL,
				&selector) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the kernel code segment selector");

  /*
   * 4)
   */

  for (i = ARCHITECTURE_IDT_EXCEPTION_BASE;
       i < ARCHITECTURE_IDT_EXCEPTION_BASE + ARCHITECTURE_IDT_EXCEPTION_SIZE;
       i++)
    {
      if (architecture_idt_insert(i,
				  (t_vaddr)_architecture_handler_shells[i],
				  selector,
				  ARCHITECTURE_IDTE_DPL_SET(
				    ARCHITECTURE_PRIVILEGE_RING0) |
				  ARCHITECTURE_IDTE_32BIT |
				  ARCHITECTURE_IDTE_INTERRUPT) != STATUS_OK)
	MACHINE_ESCAPE("unable to register the exception handler '%u'",
		       i);
    }

  /*
   * 5)
   */

  for (i = ARCHITECTURE_IDT_IRQ_BASE;
       i < ARCHITECTURE_IDT_IRQ_BASE + ARCHITECTURE_IDT_IRQ_SIZE;
       i++)
    {
      if (architecture_idt_insert(i,
				  (t_vaddr)_architecture_handler_shells[i],
				  selector,
				  ARCHITECTURE_IDTE_DPL_SET(
				    ARCHITECTURE_PRIVILEGE_RING0) |
				  ARCHITECTURE_IDTE_32BIT |
				  ARCHITECTURE_IDTE_INTERRUPT) != STATUS_OK)
	MACHINE_ESCAPE("unable to register the exception handler '%u'",
		       i);
    }

  /*
   * 6)
   */

  for (i = ARCHITECTURE_IDT_SYSCALL_BASE;
       i < ARCHITECTURE_IDT_SYSCALL_BASE + ARCHITECTURE_IDT_SYSCALL_SIZE;
       i++)
    {
      if (architecture_idt_insert(i,
				  (t_vaddr)_architecture_handler_shells[i],
				  selector,
				  ARCHITECTURE_IDTE_DPL_SET(
				    ARCHITECTURE_PRIVILEGE_RING3) |
				  ARCHITECTURE_IDTE_32BIT |
				  ARCHITECTURE_IDTE_INTERRUPT) != STATUS_OK)
	MACHINE_ESCAPE("unable to register the exception handler '%u'",
		       i);
    }

  MACHINE_LEAVE();
}

/*
 * this function is called by handlers whenever an event is received which
 * does not have a handler associated with.
 *
 * steps:
 *
 * 1) locate the context within the interrupted thread's address space.
 * 2) retrieve the current thread identifier.
 * 3) retrieve the current context.
 * 4) display the interrupt information and context.
 * 5) retrieve the current thread and task objects.
 * 6) read some of the thread's stack.
 * 7) display the read portion of the stack.
 */

void			architecture_handler_spurious(t_uint32	n)
{
  i_thread		id;
  o_thread*		thread;
  o_task*		task;
  t_uint32		stack[8];
  as_context		ctx;

  /*
   * 2)
   */

  assert(thread_current(&id) == STATUS_OK);

  /*
   * 3)
   */

  assert(architecture_context_get(id, &ctx) == STATUS_OK);

  /*
   * 4)
   */

  module_call(console, message,
	      '!',
	      "spurious: n(%u) thread(%qu)\n",
	      n, id);

  module_call(console, message,
	      '!',
	      "  eip(0x%08x) ebp(0x%08x) esp(0x%08x) eflags(0x%08x)\n",
	      ctx.eip, ctx.ebp, ctx.esp, ctx.eflags);

  module_call(console, message,
	      '!',
	      "  cs(0x%x) ds(0x%x) ss(0x%x)\n",
	      ctx.cs, ctx.ds, ctx.ss);

  module_call(console, message,
	      '!',
	      "  eax(0x%x) ebx(0x%x) ecx(0x%x) edx(0x%x) "
	      "esi(0x%x) edi(0x%x)\n",
	      ctx.eax, ctx.ebx, ctx.ecx, ctx.edx, ctx.esi, ctx.edi);

  /*
   * 5)
   */

  assert(thread_get(id, &thread) == STATUS_OK);

  assert(task_get(thread->task, &task) == STATUS_OK);

  /*
   * 6)
   */

  assert(as_read(task->as, ctx.esp, sizeof (stack), stack) == STATUS_OK);

  /*
   * 7)
   */

  module_call(console, message,
	      '!',
	      "  stack: \n"
	      "    0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
	      stack[0], stack[1], stack[2], stack[3],
	      stack[4], stack[5], stack[6], stack[7]);
}

/*
 * this function is the exception handler, called by the exception shell.
 *
 * steps:
 *
 * 1) locate the context within the interrupted thread's address space.
 * 2) if this event has not been registered, trigger a spurious interrupt.
 * 3) retrieve the event object.
 * 4) set the exception error code in case the high-level handler needs it.
 * 5) notify the event manager so that the high-level handler gets triggered.
 * 6) re-initialize the exception error code to zero.
 */

void			architecture_handler_exception(t_uint32	n,
						       t_uint32	code)
{
  i_event		id;
  o_event*		o;

  /*
   * 1)
   */

  assert(architecture_context_locate() == STATUS_OK);

  /*
   * 2)
   */

  id = (i_event)(ARCHITECTURE_IDT_EXCEPTION_BASE + n);

  if (event_exist(id) != TRUE)
    {
      architecture_handler_spurious((t_uint32)id);

      return;
    }

  /*
   * 3)
   */

  assert(event_get(id, &o) == STATUS_OK);

  /*
   * 4)
   */

  _architecture.error = code;

  /*
   * 5)
   */

  assert(event_notify(id) == STATUS_OK);

  /*
   * 6)
   */

  _architecture.error = 0;
}

/*
 * this function is the IRQ handler, called by the IRQ shell.
 *
 * steps:
 *
 * 1) locate the context within the interrupted thread's address space.
 * 2) if this event has not been registered, trigger a spurious interrupt.
 * 3) retrieve the event object.
 * 4) notify the event manager so that the high-level handler gets triggered.
 * 5) acknowledge the interrupt through the PIC.
 */

void			architecture_handler_irq(t_uint32	n)
{
  i_event		id;
  o_event*		o;

  /*
   * 1)
   */

  assert(architecture_context_locate() == STATUS_OK);

  /*
   * 2)
   */

  id = (i_event)(ARCHITECTURE_IDT_IRQ_BASE + n);

  if (event_exist(id) != TRUE)
    {
      architecture_handler_spurious((t_uint32)id);

      return;
    }

  /*
   * 3)
   */

  assert(event_get(id, &o) == STATUS_OK);

  /*
   * 4)
   */

  assert(event_notify(id) == STATUS_OK);

  /*
   * 5)
   */

  platform_pic_acknowledge(n);
}

/*
 * this function is the syscall handler, called by the syscall shell.
 *
 * steps:
 *
 * 1) locate the context within the interrupted thread's address space.
 * 2) if this event has not been registered, trigger a spurious interrupt.
 * 3) retrieve the event object.
 * 4) notify the event manager so that the high-level handler gets triggered.
 */

void			architecture_handler_syscall(t_uint32	n)
{
  i_event		id;
  o_event*		o;

  /*
   * 1)
   */

  assert(architecture_context_locate() == STATUS_OK);

  /*
   * 2)
   */

  id = (i_event)(ARCHITECTURE_IDT_SYSCALL_BASE + n);

  if (event_exist(id) != TRUE)
    {
      architecture_handler_spurious((t_uint32)id);

      return;
    }

  /*
   * 3)
   */

  assert(event_get(id, &o) == STATUS_OK);

  /*
   * 4)
   */

  assert(event_notify(id) == STATUS_OK);
}

/*
 * these macro-function calls generates the handler shells for all the
 * supported exceptions, IRQs and syscalls.
 *
 * note once again that the IPI - Inter-Process Interrupts are ignored as
 * not supported by this architecture implementation.
 */

ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(0);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(1);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(2);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(3);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(4);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(5);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(6);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(7);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_CODE(8);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(9);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_CODE(10);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_CODE(11);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_CODE(12);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_CODE(13);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_CODE(14);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(15);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(16);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_CODE(17);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(18);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(19);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(20);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(21);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(22);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(23);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(24);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(25);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(26);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(27);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(28);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(29);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(30);
ARCHITECTURE_HANDLER_SHELL_EXCEPTION_NOCODE(31);

ARCHITECTURE_HANDLER_SHELL_IRQ(0);
ARCHITECTURE_HANDLER_SHELL_IRQ(1);
ARCHITECTURE_HANDLER_SHELL_IRQ(2);
ARCHITECTURE_HANDLER_SHELL_IRQ(3);
ARCHITECTURE_HANDLER_SHELL_IRQ(4);
ARCHITECTURE_HANDLER_SHELL_IRQ(5);
ARCHITECTURE_HANDLER_SHELL_IRQ(6);
ARCHITECTURE_HANDLER_SHELL_IRQ(7);
ARCHITECTURE_HANDLER_SHELL_IRQ(8);
ARCHITECTURE_HANDLER_SHELL_IRQ(9);
ARCHITECTURE_HANDLER_SHELL_IRQ(10);
ARCHITECTURE_HANDLER_SHELL_IRQ(11);
ARCHITECTURE_HANDLER_SHELL_IRQ(12);
ARCHITECTURE_HANDLER_SHELL_IRQ(13);
ARCHITECTURE_HANDLER_SHELL_IRQ(14);
ARCHITECTURE_HANDLER_SHELL_IRQ(15);

ARCHITECTURE_HANDLER_SHELL_SYSCALL(0);
ARCHITECTURE_HANDLER_SHELL_SYSCALL(1);
ARCHITECTURE_HANDLER_SHELL_SYSCALL(2);
ARCHITECTURE_HANDLER_SHELL_SYSCALL(3);
ARCHITECTURE_HANDLER_SHELL_SYSCALL(4);
ARCHITECTURE_HANDLER_SHELL_SYSCALL(5);
ARCHITECTURE_HANDLER_SHELL_SYSCALL(6);
ARCHITECTURE_HANDLER_SHELL_SYSCALL(7);
ARCHITECTURE_HANDLER_SHELL_SYSCALL(8);
ARCHITECTURE_HANDLER_SHELL_SYSCALL(9);
