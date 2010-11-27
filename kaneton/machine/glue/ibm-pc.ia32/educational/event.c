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
 * updated       julien quintard   [sat nov 27 16:20:23 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
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
 * the event manager dispatch.
 */

d_event			glue_event_dispatch =
  {
    NULL,
    glue_event_enable,
    glue_event_disable,
    NULL,
    glue_event_reserve,
    glue_event_release,
    glue_event_initialize,
    glue_event_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

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

  STI();

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

  CLI();

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
  if (id >= IA32_HANDLER_NR)
    MACHINE_ESCAPE("the event identifier '%qu' is larger than the higher "
		   "syscall number that the kernel is supposed to handle",
		   id);

  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    {
      if (platform_irq_enable(id - IA32_IDT_IRQ_BASE) != ERROR_OK)
	MACHINE_ESCAPE("unable to enable the IRQ");
    }

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_event_release(i_event		id)
{
  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    {
      if (platform_irq_disable(id - IA32_IDT_IRQ_BASE) != ERROR_OK)
	MACHINE_ESCAPE("unable to disable the IRQ");
    }

  MACHINE_LEAVE();
}

// XXX a renommer
void			pf_handler(t_id				id,
				   t_vaddr			data,
				   t_uint32			error_code)
{
  i_thread		th;
  t_uint32              addr;
  t_ia32_context	ctx;

  assert(scheduler_current(&th) == ERROR_OK);

  assert(ia32_get_context(th, &ctx) == ERROR_OK);

  SCR2(addr);

  printf("error: page fault !\n"
         "  %p trying to %s at the address 0x%x requires %s\n",
	 ctx.eip,
         (error_code & 2) ? "write" : "read",
         addr,
         (error_code & 1) ? "a lower DPL" : "the page to be present");

  ia32_print_context(th);

  while (1)
    ;
}

/*
 * XXX
 */

t_error			glue_event_initialize(void)
{
  if (ia32_interrupt_vector_init() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the interrupt table");

  if (platform_irq_initialize() != ERROR_OK)
    MACHINE_ESCAPE("unable to initialize the IRQs");

  if (event_reserve(14, EVENT_FUNCTION,
		    EVENT_HANDLER(pf_handler), 0) != ERROR_OK)
    MACHINE_ESCAPE("unable to reserve the event associated with the "
		   "page fault exception");

  MACHINE_LEAVE();
}

/*
 * XXX
 */

t_error			glue_event_clean(void)
{
  if (platform_irq_clean() != ERROR_KO)
    MACHINE_ESCAPE("unable to clean the IRQs");

  MACHINE_LEAVE();
}
