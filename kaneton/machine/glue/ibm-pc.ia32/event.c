/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/machine/glue/ibm-pc.ia32/event.c
 *
 * created       renaud voltz   [mon feb 13 01:05:52 2006]
 * updated       julien quintard   [fri jun 22 18:32:51 2007]
 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_event*		event;

/*
 * the event manager dispatch.
 */

d_event				event_dispatch =
  {
/*								    [cut] k2 */
    NULL,
    NULL,
    glue_event_reserve,
    glue_event_release,
    glue_event_initialize,
    glue_event_clean
/*								   [cut] /k2 */
  };

/*								    [cut] k2 */

/*
 * ---------- functions -------------------------------------------------------
 */

t_error			glue_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler)
{
  EVENT_ENTER(event);

  if (id >= IA32_HANDLER_NR)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    if (ibmpc_enable_irq(id - IA32_IDT_IRQ_BASE) != ERROR_NONE)
      EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

t_error			glue_event_release(i_event		id)
{
  EVENT_ENTER(event);

  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    if (ibmpc_disable_irq(id - IA32_IDT_IRQ_BASE) != ERROR_NONE)
      EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

static void		pf_handler(t_id				id,
				   t_uint32			error_code)
{
  t_uint32              addr;

  SCR2(addr);

  printf("error: page fault !\n"
         "  %p trying to %s at the address 0x%x requires %s\n",
	 context->eip,
         (error_code & 2) ? "write" : "read",
         addr,
         (error_code & 1) ? "a lower DPL" : "the page to be present");

  while (1)
    ;
}

t_error			glue_event_initialize(void)
{
  EVENT_ENTER(event);

  if (ia32_interrupt_vector_init() != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  if (ibmpc_irq_init() != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  if (event_reserve(14, EVENT_FUNCTION,
		    EVENT_HANDLER(pf_handler)) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

t_error			glue_event_clean(void)
{
  EVENT_ENTER(event);

  CLI();

  EVENT_LEAVE(event, ERROR_NONE);
}
