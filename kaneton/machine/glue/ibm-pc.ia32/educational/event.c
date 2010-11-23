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
 * updated       julien quintard   [mon nov 22 22:24:11 2010]
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

/*
 * the event manager.
 */

extern m_event*		_event;

/*
 * ---------- globals ---------------------------------------------------------
 */

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

t_error			glue_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler,
					   t_vaddr		data)
{
  EVENT_ENTER(_event);

  if (id >= IA32_HANDLER_NR)
    EVENT_LEAVE(_event, ERROR_KO);

  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    if (ibmpc_irq_enable(id - IA32_IDT_IRQ_BASE) != ERROR_OK)
      EVENT_LEAVE(_event, ERROR_KO);

  EVENT_LEAVE(_event, ERROR_OK);
}

t_error			glue_event_release(i_event		id)
{
  EVENT_ENTER(_event);

  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    if (ibmpc_irq_disable(id - IA32_IDT_IRQ_BASE) != ERROR_OK)
      EVENT_LEAVE(_event, ERROR_KO);

  EVENT_LEAVE(_event, ERROR_OK);
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

t_error			glue_event_initialize(void)
{
  EVENT_ENTER(_event);

  if (ia32_interrupt_vector_init() != ERROR_OK)
    EVENT_LEAVE(_event, ERROR_KO);

  if (ibmpc_irq_init() != ERROR_OK)
    EVENT_LEAVE(_event, ERROR_KO);

  if (event_reserve(14, EVENT_FUNCTION,
		    EVENT_HANDLER(pf_handler), 0) != ERROR_OK)
    EVENT_LEAVE(_event, ERROR_KO);

  EVENT_LEAVE(_event, ERROR_OK);
}

t_error			glue_event_clean(void)
{
  EVENT_ENTER(_event);

  EVENT_LEAVE(_event, ERROR_OK);
}
