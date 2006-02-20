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
 * ---------- extern ----------------------------------------------------------
 */

extern m_event*			event;

extern t_interrupt_handler      exception_handlers[32];

extern t_interrupt_handler      irq_handlers[16];


/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager interface.
 */

i_event                  event_interface =
  {

    /*								[cut] k3 */

    ia32_event_subscribe,
    ia32_event_unsubscribe,
    ia32_event_init,
    ia32_event_clean

    /*								[cut] /k3 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX EVENT
 */

t_error			ia32_event_subscribe(void)
{

  return ERROR_NONE;
}

/*
 * XXX EVENT
 */

t_error			ia32_event_unsubscribe(void)
{

  return ERROR_NONE;
}

/*
 * XXX EVENT
 *
 * steps:
 *
 * 1) init exceptions
 * 2) init irq
 */

t_error			ia32_event_init(void)
{
  /*
   * 1)
   */

  if (exception_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (irq_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  ia32_event_add(33, 0, XXX);

  return ERROR_NONE;
}

t_error			ia32_event_clean(void)
{

  return ERROR_NONE;
}


/*
 * XXX EVENT
 *
 * steps:
 *
 * 1) check id bounds
 * 2) add an exception handler
 * 3) add an irq handler
 * 4) XXX just for debug
 */

t_error			ia32_event_add(t_uint16			id,
				       t_prvl			privilege,
				       t_interrupt_handler	handler)
{
  /*
   * 1)
   */

  if (id >= IDT_MAX_ENTRIES)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if ((id >= IDT_EXCEPTION_BASE) && (id < IDT_EXCEPTION_BASE + 32))
    {
      exception_handlers[id] = handler;

      return ERROR_NONE;
    }

  /*
   * 3)
   */

  if ((id >= IDT_IRQ_BASE) && (id < IDT_IRQ_BASE + 16))
    {
      irq_handlers[id - IDT_IRQ_BASE] = handler;

      pic_enable_irq(id - IDT_IRQ_BASE);

      return ERROR_NONE;
    }

  /*
   * 4)
   */

  printf("ia32_event: id does not match neither an exception or an irq\n");

  return ERROR_UNKNOWN;
}

/*
 * XXX EVENT (remove me)
 * fake handler, just for testing ...
 */

void	XXX(void)
{
  printf("kbd interrupt\n");
}
