/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/XYZ/...chine/glue/ibm-pc.ia32/optimised/event.c
 *
 * created       renaud voltz   [mon feb 13 01:05:52 2006]
 * updated       julien quintard   [tue nov 23 20:13:54 2010]
 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- includes --------------------------------------------------------
 */

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
    NULL,
    NULL,
    glue_event_reserve,
    glue_event_release,
    glue_event_initialize,
    glue_event_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

t_status		glue_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler,
					   t_vaddr		data)
{
  EVENT_ENTER(event);

  if (id >= IA32_HANDLER_NR)
    EVENT_LEAVE(event, STATUS_UNKNOWN_ERROR);

  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    if (ibmpc_enable_irq(id - IA32_IDT_IRQ_BASE) != STATUS_OK)
      EVENT_LEAVE(event, STATUS_UNKNOWN_ERROR);

  EVENT_LEAVE(event, STATUS_OK);
}

t_status		glue_event_release(i_event		id)
{
  EVENT_ENTER(event);

  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    if (ibmpc_disable_irq(id - IA32_IDT_IRQ_BASE) != STATUS_OK)
      EVENT_LEAVE(event, STATUS_UNKNOWN_ERROR);

  EVENT_LEAVE(event, STATUS_OK);
}

static void		pf_handler(t_id				id,
				   t_vaddr			data,
				   t_uint32			error_code)
{
  i_thread		th;
  t_uint32              addr;
  t_ia32_context	ctx;

  assert(scheduler_current(&th) == STATUS_OK);

  assert(ia32_get_context(th, &ctx) == STATUS_OK);

  SCR2(addr);

  printf("error: page fault !\n"
         "  %p trying to %s at the address 0x%x requires %s\n",
	 ctx.eip,
         (error_code & 2) ? "write" : "read",
         addr,
         (error_code & 1) ? "a lower DPL" : "the page to be present");

  /* ia32_print_context(th); */

  while (1)
    ;
}

t_status		glue_event_initialize(void)
{
  EVENT_ENTER(event);

  if (ia32_interrupt_vector_init() != STATUS_OK)
    EVENT_LEAVE(event, STATUS_UNKNOWN_ERROR);

  if (ibmpc_irq_init() != STATUS_OK)
    EVENT_LEAVE(event, STATUS_UNKNOWN_ERROR);

  if (event_reserve(14, EVENT_FUNCTION,
		    EVENT_HANDLER(pf_handler), 0) != STATUS_OK)
    EVENT_LEAVE(event, STATUS_UNKNOWN_ERROR);

  EVENT_LEAVE(event, STATUS_OK);
}

t_status		glue_event_clean(void)
{
  EVENT_ENTER(event);

  CLI();

  EVENT_LEAVE(event, STATUS_OK);
}
