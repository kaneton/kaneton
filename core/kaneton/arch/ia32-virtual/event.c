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
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>


/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_event*          event;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager interface.
 */

i_event                  event_interface =
  {

    /*                                                                  [cut] k2 */

    NULL,
    ia32_event_init,
    NULL

    /*                                                                 [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

t_error                 exception_generic(void)
{
  printf("generic exception handler\n");

  while (1);

  return ERROR_NONE;
}

/*
 *
 *
 *
 *
 *
 */

t_error			ia32_event_init(void)
{
  if (exception_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (irq_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * XXX
 *
 * steps:
 *
 * 1)
 * 2)
 * 3)
 *
 */

t_error			ia32_event_add(t_uint16			id,
				       t_prvl			privilege,
				       t_uint32			handler)
{
  t_gate		gate;

  /*
   * 1)
   */

  if (id >= IDT_MAX_ENTRIES)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  gate.offset = handler;
  gate.segsel = 1 << 3;
  gate.privilege = privilege;
  gate.type = type_gate_interrupt;

  idt_add_gate(NULL, id, gate);

  /*
   * 3)
   */

  if (id >= IDT_IRQ_BASE)
    interrupt_enable(id - IDT_IRQ_BASE);

  return ERROR_NONE;
}
