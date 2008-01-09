/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/cry...ine/glue/ibm-pc.ia32/educational/event.c
 *
 * created       renaud voltz   [mon feb 13 01:05:52 2006]
 * updated       matthieu bucchianeri   [wed jan  9 11:49:03 2008]
 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc/libc.h>
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

/*
 * [fxcut k2 code: return (ERROR_NONE);]
 */

t_error			glue_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler,
					   t_vaddr		data)
{
  EVENT_ENTER(event);

  if (id >= IA32_HANDLER_NR)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    if (ibmpc_enable_irq(id - IA32_IDT_IRQ_BASE) != ERROR_NONE)
      EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * [fxcut k2 code: return (ERROR_NONE);]
 */

t_error			glue_event_release(i_event		id)
{
  EVENT_ENTER(event);

  if (id >= IA32_IDT_IRQ_BASE && id < IA32_IDT_IRQ_BASE + IA32_IRQ_NR)
    if (ibmpc_disable_irq(id - IA32_IDT_IRQ_BASE) != ERROR_NONE)
      EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/* [cut k2] */
static void		pf_handler(t_id				id,
				   t_vaddr			data,
				   t_uint32			error_code)
{
  i_thread		th;
  t_uint32              addr;
  t_ia32_context	ctx;

  ASSERT(scheduler_current(&th) == ERROR_NONE);

  ASSERT(ia32_get_context(th, &ctx) == ERROR_NONE);


  SCR2(addr);

  printf("error: page fault !\n"
         "  %p trying to %s at the address 0x%x requires %s\n",
	 ctx.eip,
         (error_code & 2) ? "write" : "read",
         addr,
         (error_code & 1) ? "a lower DPL" : "the page to be present");

  while (1)
    ;
}
/* [/cut] */

/*
 * [fxcut k2 code: return (ERROR_NONE);]
 */

t_error			glue_event_initialize(void)
{
  EVENT_ENTER(event);

  if (ia32_interrupt_vector_init() != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  if (ibmpc_irq_init() != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  if (event_reserve(14, EVENT_FUNCTION,
		    EVENT_HANDLER(pf_handler), 0) != ERROR_NONE)
    EVENT_LEAVE(event, ERROR_UNKNOWN);

  EVENT_LEAVE(event, ERROR_NONE);
}

/*
 * [fxcut k2 code: return (ERROR_NONE);]
 */

t_error			glue_event_clean(void)
{
  EVENT_ENTER(event);

  CLI();

  EVENT_LEAVE(event, ERROR_NONE);
}
