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
 * updated       julien quintard   [mon jan 10 21:29:41 2011]
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

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event dispatcher.
 */

d_event			glue_event_dispatch =
  {
    NULL,
    NULL,
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
 * XXX
 */

void			glue_event_pagefault(t_id		id,
					     t_vaddr		data)
{
  i_thread		th;
  t_uint32              addr;
  t_ia32_context	ctx;

  printf("[XXX] glue_event:pf_handler() by %qd\n", th);

  assert(thread_current(&th) == ERROR_OK);

  assert(ia32_get_context(th, &ctx) == ERROR_OK);

  ARCHITECTURE_SCR2(addr);

  module_call(console, print,
	      "error: page fault !\n"
	      "  0x%x @ 0x%x\n",
	      ctx.eip,
	      addr);

  extern t_uint32 _architecture_handler_exception_code;

  int error_code = _architecture_handler_exception_code;

  /* XXX utiliser le error_code (soit dans o_thread) soit autre part */
  module_call(console, print,
	      "error: page fault !\n"
	      "  0x%x trying to %s at the address 0x%x requires %s\n",
	      ctx.eip,
	      (error_code & 2) ? "write" : "read",
	      addr,
	      (error_code & 1) ? "a lower DPL" : "the page to be present");
  /* */

  ia32_print_context(th);

  while (1)
    ;
}

/*
 * this function enables the interrupts, hence the event processing.
 *
 * steps:
 *
 * 1) call the STI instruction.
 */

t_error			glue_event_enable(void)
{
  /*
   * 1)
   */

  ARCHITECTURE_STI();

  MACHINE_LEAVE();
}

/*
 * this function disables the interrupts, hence the event processing.
 *
 * steps:
 *
 * 1) call the CLI instruction.
 */

t_error			glue_event_disable(void)
{
  /*
   * 1)
   */

  ARCHITECTURE_CLI();

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler,
					   t_vaddr		data)
{
  /*
   * XXX
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
   * XXX
   */

  if ((id >= ARCHITECTURE_IDT_IRQ_BASE) &&
      (id < ARCHITECTURE_IDT_IRQ_BASE + ARCHITECTURE_IDT_IRQ_SIZE))
    {
      if (platform_pic_enable(id - ARCHITECTURE_IDT_IRQ_BASE) != ERROR_OK)
	MACHINE_ESCAPE("unable to enable the IRQ");
    }

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_event_release(i_event		id)
{
  /*
   * XXX
   */

  if ((id >= ARCHITECTURE_IDT_IRQ_BASE) &&
      (id < ARCHITECTURE_IDT_IRQ_BASE + ARCHITECTURE_IDT_IRQ_SIZE))
    {
      if (platform_pic_disable(id - ARCHITECTURE_IDT_IRQ_BASE) != ERROR_OK)
	MACHINE_ESCAPE("unable to disable the IRQ");
    }

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_event_initialize(void)
{
  /*
   * XXX
   */

  if (architecture_handler_setup() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the interrupt table");

  /*
   * XXX
   */

  if (platform_pic_initialize() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the PIC");

  /*
   * XXX
   */

  if (event_reserve(14,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(glue_event_pagefault),
		    EVENT_DATA(NULL)) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the event associated with the "
		   "page fault exception");

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_event_clean(void)
{
  if (platform_pic_clean() != ERROR_KO)
    MACHINE_ESCAPE("unable to clean the PIC");

  MACHINE_LEAVE();
}
