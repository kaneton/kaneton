/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ine/glue/ibm-pc.ia32/educational/event.c
 *
 * created       renaud voltz   [mon feb 13 01:05:52 2006]
 * updated       julien quintard   [wed mar  2 20:28:13 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the event manager's glue.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the event manager.
 */

extern m_event		_event;

/*
 * the architecture manager.
 */

extern am		_architecture;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event dispatcher.
 */

d_event			glue_event_dispatch =
  {
    NULL,
    glue_event_dump,
    NULL,
    glue_event_enable,
    glue_event_disable,
    glue_event_reserve,
    glue_event_release,
    glue_event_initialize,
    glue_event_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this handler is triggered whenever a page fault---i.e an access to a
 * invalid virtual address---occurs.
 *
 * steps:
 *
 * 1) retrieve the currently running thread identifier.
 * 2) retrieve the thread's IA32 context.
 * 3) retrieve the content of the CR2 register i.e the address of
 *    access which caused the page fault.
 * 4) display information on the page fault.
 * 5) show the context.
 */

void			glue_event_pagefault(t_id		id,
					     t_data		data)
{
  i_thread		thread;
  t_uint32              address;
  as_context		ctx;

  /*
   * 1)
   */

  assert(thread_current(&thread) == STATUS_OK);

  /*
   * 2)
   */

  assert(architecture_context_get(thread, &ctx) == STATUS_OK);

  /*
   * 3)
   */

  ARCHITECTURE_SCR2(address);

  /*
   * 4)
   */

  module_call(console, message,
	      '!', "page fault: thread(%qd) eip(0x%08x) address(0x%08x) "
	      "access(%s) mode(%s) reason(%s)\n",
	      thread,
	      ctx.eip,
	      address,
	      (_architecture.error &
	       ARCHITECTURE_HANDLER_PAGEFAULT_WRITE) ?
	      "write" : "read",
	      (_architecture.error &
	       ARCHITECTURE_HANDLER_PAGEFAULT_USER) ?
	      "user" : "supervisor",
	      (_architecture.error &
	       ARCHITECTURE_HANDLER_PAGEFAULT_PRIVILEGE) ?
	      "protection violation" : "non-present page");

  /*
   * 5)
   */

  module_call(console, message,
	      '!',
	      "  context: eax(0x%08x) ebx(0x%08x) ecx(0x%08x) "
	      "edx(0x%08x) esi(0x%08x) edi(0x%08x) "
	      "ebp(0x%08x) esp(0x%08x) eip(0x%08x)\n",
	      ctx.eax, ctx.ebx, ctx.ecx,
	      ctx.edx, ctx.esi, ctx.edi,
	      ctx.ebp, ctx._esp, ctx.eip);

  /*
   * 6)
   */

  while (1)
    ;
}

/*
 * this function displays additional machine-specific information on
 * the event manager.
 *
 * steps:
 *
 * 1) display the machine-specific attributes.
 */

t_status		glue_event_dump(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '#',
	      "  machine:\n");

  module_call(console, message,
	      '#',
	      "    idt: table(0x%x) size(0x%x)\n",
	      _event.machine.idt.table,
	      _event.machine.idt.size);

  MACHINE_LEAVE();
}

/*
 * this function enables the interrupts, hence the event processing.
 *
 * note that this function must triggers an event right away.
 *
 * steps:
 *
 * 1) call the STI instruction.
 * 2) manually trigger an event.
 */

t_status		glue_event_enable(void)
{
  /*
   * 1)
   */

  ARCHITECTURE_STI();

  /*
   * 2)
   */

  asm volatile ("int $32");

  MACHINE_LEAVE();
}

/*
 * this function disables the interrupts, hence the event processing.
 *
 * steps:
 *
 * 1) call the CLI instruction.
 */

t_status		glue_event_disable(void)
{
  /*
   * 1)
   */

  ARCHITECTURE_CLI();

  MACHINE_LEAVE();
}

/*
 * this function activates the hardware-related event, if necessary.
 *
 * steps:
 *
 * 0) verify the arguments.
 * 1) if the reserved event is an IRQ - Interrupt Request, activate it
 *    via the PIC - Programmable Interrupt Controller.
 */

t_status		glue_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler,
					   t_data		data)
{
  /*
   * 0)
   */

  if (!(((id >= ARCHITECTURE_IDT_EXCEPTION_BASE) &&
	 (id < (ARCHITECTURE_IDT_EXCEPTION_BASE +
	       ARCHITECTURE_IDT_EXCEPTION_SIZE))) ||
	((id >= ARCHITECTURE_IDT_IRQ_BASE) &&
	 (id < (ARCHITECTURE_IDT_IRQ_BASE +
		ARCHITECTURE_IDT_IRQ_SIZE))) ||
	((id >= ARCHITECTURE_IDT_SYSCALL_BASE) &&
	 (id < (ARCHITECTURE_IDT_SYSCALL_BASE +
		ARCHITECTURE_IDT_SYSCALL_SIZE)))))
    MACHINE_ESCAPE("this event identifier '%qu' is not supported",
		   id);

  /*
   * 1)
   */

  if ((id >= ARCHITECTURE_IDT_IRQ_BASE) &&
      (id < ARCHITECTURE_IDT_IRQ_BASE + ARCHITECTURE_IDT_IRQ_SIZE))
    {
      if (platform_pic_enable(id - ARCHITECTURE_IDT_IRQ_BASE) != STATUS_OK)
	MACHINE_ESCAPE("unable to enable the IRQ");
    }

  MACHINE_LEAVE();
}

/*
 * this function releases an event.
 *
 * steps:
 *
 * 1) should the event be an IRQ, deactivate it.
 */

t_status		glue_event_release(i_event		id)
{
  /*
   * 1)
   */

  if ((id >= ARCHITECTURE_IDT_IRQ_BASE) &&
      (id < ARCHITECTURE_IDT_IRQ_BASE + ARCHITECTURE_IDT_IRQ_SIZE))
    {
      if (platform_pic_disable(id - ARCHITECTURE_IDT_IRQ_BASE) != STATUS_OK)
	MACHINE_ESCAPE("unable to disable the IRQ");
    }

  MACHINE_LEAVE();
}

/*
 * this function initializes the hardware interrupt handling mechanism.
 *
 * steps:
 *
 * 1) set up the handler system.
 * 2) initialize the PIC.
 * 3) reserve the page fault event and register its handler.
 */

t_status		glue_event_initialize(void)
{
  /*
   * 1)
   */

  if (architecture_handler_setup() != STATUS_OK)
    MACHINE_ESCAPE("unable to initialize the interrupt table");

  /*
   * 2)
   */

  if (platform_pic_initialize() != STATUS_OK)
    MACHINE_ESCAPE("unable to initialize the PIC");

  /*
   * 3)
   */

  if (event_reserve(ARCHITECTURE_IDT_EXCEPTION_PF,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(glue_event_pagefault),
		    EVENT_DATA(NULL)) != STATUS_OK)
    MACHINE_ESCAPE("unable to reserve the event associated with the "
		   "page fault exception");

  MACHINE_LEAVE();
}

/*
 * this function cleans the hardware event processing.
 *
 * steps:
 *
 * 1) clean the PIC.
 */

t_status		glue_event_clean(void)
{
  /*
   * 1)
   */

  if (platform_pic_clean() != STATUS_ERROR)
    MACHINE_ESCAPE("unable to clean the PIC");

  MACHINE_LEAVE();
}
