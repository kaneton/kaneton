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
 * this file implements dependent code for the events on ia32 with paging
 * architecture.
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX EVENT assignments have to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*								    [cut] k3 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager interface.
 */

i_event				event_interface =
  {
    NULL,
    NULL,
    ia32_event_reserve,
    ia32_event_release,
    ia32_event_init,
    ia32_event_clean
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
 * 2) associate the handler to the given eventid.
 * 3) unmask event if eventid is an irq.
 */

t_error			ia32_event_reserve(t_eventid		eventid,
					   e_event_type		type,
					   u_event_handler	handler)
{
  t_uint32		id = eventid;

  /*
   * 1)
   */

  if (id >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (type == EVENT_MESSAGE)
    {
      if (interrupt_set_handler(id, (t_event_handler)event_notify)
	  != ERROR_NONE)
	return ERROR_UNKNOWN;
    }
  else
    {
      if (interrupt_set_handler(id, handler.function) != ERROR_NONE)
	return ERROR_UNKNOWN;
    }

  /*
   * 3)
   */

  if ((id >= IDT_IRQ_BASE) && (id < IDT_IRQ_BASE + IRQ_NR))
    if (pic_enable_irq(id - IDT_IRQ_BASE) != ERROR_NONE)
      return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * release an event on the ia32 architecture.
 *
 * steps:
 *
 * 1) check id bounds.
 * 2) clear the eventid handler.
 * 3) mask event if eventid is an irq.
 */

t_error			ia32_event_release(t_eventid		eventid)
{
  t_uint32		id = eventid;

  /*
   * 1)
   */

  if (id >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (interrupt_set_handler(id, ia32_event_handler) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  if ((id >= IDT_IRQ_BASE) && (id < IDT_IRQ_BASE + IRQ_NR))
    if (pic_disable_irq(id - IDT_IRQ_BASE) != ERROR_NONE)
      return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * initialize events on the ia32 architecture.
 *
 * steps:
 *
 * 1) init events.
 * 2) set default handler for every exception.
 * 3) set default handler for every irq.
 * 4) enable external interrupts.
 */

t_error			ia32_event_init(void)
{
  int			id;

  /*
   * 1)
   */

  if (interrupt_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  for (id = IDT_EXCEPTION_BASE; id < IDT_EXCEPTION_BASE + EXCEPTION_NR; id++)
    if (interrupt_set_handler(id, ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 3)
   */

  for (id = IDT_IRQ_BASE; id < IDT_IRQ_BASE + IRQ_NR; id++)
    if (interrupt_set_handler(id, ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 4)
   */

  STI();

  return ERROR_NONE;
}

/*
 * this function cleans events on ia32 architecture.
 *
 * 1) ignore external interrupts.
 * 2) disable exceptions.
 * 3) disable irq's.
 */

t_error			ia32_event_clean(void)
{
  int			id;

  /*
   * 1)
   */

  CLI();

  /*
   * 2)
   */

  for (id = IDT_EXCEPTION_BASE; id < IDT_EXCEPTION_BASE + EXCEPTION_NR; id++)
    if (event_release(id) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 3)
   */

  for (id = IDT_IRQ_BASE; id < IDT_IRQ_BASE + IRQ_NR; id++)
    if (event_release(id) != ERROR_NONE)
      return ERROR_UNKNOWN;

  return ERROR_NONE;
}


/*
 * the default handler for ia32 architecture.
 *
 * do nothing.
 * used before the event has been reserved,
 * or after the event has been released.
 */

void			ia32_event_handler(t_uint32		id)
{


}

/*
 * XXX EVENT remove me !
 * just for testing.
 */

static const char       scancodes[] =
  {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', 0, 0, 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';',
    '\'', '`', '-', '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, 0, 0, ' ', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };


void                    ia32_kbd_handler(t_uint32                    id)
{
  t_uint8               scancode;

  INB(0x60, scancode);

  if (scancode < 70)
    printf("%c", scancodes[scancode]);
}

void                    ia32_pf_handler(t_uint32                     error_code)
{
  t_uint32              addr;

  SCR2(addr);

  printf("error: page fault !\n"
         "  trying to %s at the address 0x%x requires %s\n",
         (error_code & 2) ? "write" : "read",
         addr,
         (error_code & 1) ? "a lower DPL" : "the page to be present");

  while (1);
}

/*								[cut] /k3 */
