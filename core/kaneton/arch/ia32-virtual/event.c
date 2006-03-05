/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/core/kaneton/arch/ia32-virtual/event.c
 *
 * created       renaud voltz   [mon feb 13 01:05:52 2006]
 * updated       renaud voltz   [mon feb 13 01:05:52 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependant code for the events on ia32 with paging
 * architecture.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX EVENT
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager interface.
 */

i_event				event_interface =
  {

    /*								[cut] k3 */

    ia32_event_reserve,
    ia32_event_release,
    ia32_event_init,
    ia32_event_clean

    /*								[cut] /k3 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * reserve an event on the ia32 architecture.
 *
 * steps:
 *
 * 1) check id bounds.
 * 2) unmask hardware interrupt if id is an irq.
 */

t_error			ia32_event_reserve(t_eventid		id)
{
  /*
   * 1)
   */

  if (id >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if ((id >= IDT_IRQ_BASE) && (id < IDT_IRQ_BASE + IRQ_NR))
    pic_enable_irq(id - IDT_IRQ_BASE);

  return ERROR_NONE;
}

/*
 * release an event on the ia32 architecture.
 *
 * steps:
 *
 * 1) check id bounds.
 * 2) mask hardware interrupt if needed.
 */

t_error			ia32_event_release(t_eventid		id)
{
  /*
   * 1)
   */

  if (id >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if ((id >= IDT_IRQ_BASE) && (id < IDT_IRQ_BASE + IRQ_NR))
    pic_disable_irq(id - IDT_IRQ_BASE);

  return ERROR_NONE;
}

/*
 * initialize events on the ia32 architecture.
 *
 * steps:
 *
 * 1) init interrupts.
 * 3) set default handler for every event.
 */

t_error			ia32_event_init(void)
{
  int			i;

  /*
   * 1)
   */

  if (interrupt_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (interrupt_set_handler(ia32_generic_handler) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  for (i = 0; i < EXCEPTION_NR + IRQ_NR; i++)
    event_reserve(i);

  return ERROR_NONE;
}

/*
 * this function cleans the machine-dependent event manager.
 */

t_error			ia32_event_clean(void)
{
  int			i;

  for (i = 0; i < EXCEPTION_NR + IRQ_NR; i++)
    event_release(i);

  return ERROR_NONE;
}

/*
 * generic handler for ia32 architecture events.
 */

void			ia32_generic_handler(t_uint32		nr)
{
  t_eventid		eventid = nr;

  event_notify(eventid);
}
