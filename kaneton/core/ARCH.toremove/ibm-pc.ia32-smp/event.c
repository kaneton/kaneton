/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-smp/event.c
 *
 * created       renaud voltz   [mon feb 13 01:05:52 2006]
 * updated       matthieu bucchianeri   [wed mar 21 23:39:16 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependent code for the events on ia32 with paging
 * architecture.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*								    [cut] k2 */

/*
 * ---------- extern ---------------------------------------------------------
 */

/*
 * the kernel address space.
 */

extern i_as			kasid;

/*
 * the init structure.
 */

extern t_init*			init;

/*								   [cut] /k2 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager dispatch.
 */

d_event				event_dispatch =
  {

/*								    [cut] k2 */

    NULL,
    NULL,
    ia32_event_reserve,
    ia32_event_release,
    ia32_event_init,
    ia32_event_clean

/*								   [cut] /k2 */

  };

/*								    [cut] k2 */

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

t_error			ia32_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler)
{
  t_uint32		eventid = id;

  /*
   * 1)
   */

  if (eventid >= EXCEPTION_NR + IRQ_NR + IPI_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (type == EVENT_MESSAGE)
    {
      if (interrupt_set_handler(eventid, (t_ia32_interrupt_handler)event_notify)
	  != ERROR_NONE)
	return ERROR_UNKNOWN;
    }
  else
    {
      if (interrupt_set_handler(eventid, (t_ia32_interrupt_handler)handler.function) != ERROR_NONE)
	return ERROR_UNKNOWN;
    }

  /*
   * 3)
   */

  if ((eventid >= IDT_IRQ_BASE) && (eventid < IDT_IRQ_BASE + IRQ_NR))
    if (pic_enable_irq(eventid - IDT_IRQ_BASE) != ERROR_NONE)
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

t_error			ia32_event_release(i_event		id)
{
  t_uint32		eventid = id;

  /*
   * 1)
   */

  if (eventid >= EXCEPTION_NR + IRQ_NR + IPI_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (interrupt_set_handler(eventid, (t_ia32_interrupt_handler)
			    ia32_event_handler) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  if ((eventid >= IDT_IRQ_BASE) && (eventid < IDT_IRQ_BASE + IRQ_NR))
    if (pic_disable_irq(eventid - IDT_IRQ_BASE) != ERROR_NONE)
      return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * initialize events on the ia32 architecture.
 *
 * steps:
 *
 * 1) activate the IDT.
 * 2) init events.
 * 3) disable writing the IDT.
 * 4) set default handler for every exception.
 * 5) set default handler for every irq.
 * 6) set default handler for every ipi.
 * 7) enable external interrupts.
 * 8) setup default exception handlers.
 */

t_error			ia32_event_init(void)
{
  int			id;
  o_region*		oreg;

  /*
   * 1)
   */

  if (idt_activate(&init->machdep.idt) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (interrupt_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  if (region_get(kasid, (i_region)(t_uint32)init->machdep.idt.descriptor,
		 &oreg) != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (segment_perms(oreg->segid, PERM_READ) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 4)
   */

  for (id = IDT_EXCEPTION_BASE; id < IDT_EXCEPTION_BASE + EXCEPTION_NR; id++)
    if (interrupt_set_handler(id, (t_ia32_interrupt_handler)
			      ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 5)
   */

  for (id = IDT_IRQ_BASE; id < IDT_IRQ_BASE + IRQ_NR; id++)
    if (interrupt_set_handler(id, (t_ia32_interrupt_handler)
			      ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 6)
   */

  for (id = IDT_IPI_BASE; id < IDT_IPI_BASE + IPI_NR; id++)
    if (interrupt_set_handler(id, (t_ia32_interrupt_handler)
			      ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 7)
   */

  STI();

  /*
   * 8)
   */

  if (event_reserve(33, EVENT_FUNCTION, EVENT_HANDLER(ia32_kbd_handler))
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (event_reserve(14, EVENT_FUNCTION, EVENT_HANDLER(ia32_pf_handler))
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (event_reserve(13, EVENT_FUNCTION, EVENT_HANDLER(ia32_gp_handler))
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (event_reserve(8, EVENT_FUNCTION, EVENT_HANDLER(ia32_df_handler))
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (event_reserve(10, EVENT_FUNCTION, EVENT_HANDLER(ia32_ts_handler))
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (event_reserve(7, EVENT_FUNCTION, EVENT_HANDLER(ia32_sched_switch_mmx))
      != ERROR_NONE)
    return ERROR_UNKNOWN;

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

void			ia32_event_handler(t_id		id)
{
  printf("unhandled exception... %d\n", id);

  while(1)
    ;
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


void                    ia32_kbd_handler(t_id			id)
{
  t_uint8               scancode;

  INB(0x60, scancode);

  if (scancode < 70)
    {
      switch (scancodes[scancode])
	{
	  case 's':
	    printf("\n");
	    STATS_DUMP();
	    printf("\n");
	    break;
	  case 'c':
	    printf("\n");
	    sched_dump();
	    printf("\n");
	    break;
	  default:
	    printf("%c", scancodes[scancode]);
	}
    }
}





typedef struct
{
  t_uint32	fs;
  t_uint32	es;
  t_uint32	ds;
  t_uint32	esi;
  t_uint32	edi;
  t_uint32	edx;
  t_uint32	ecx;
  t_uint32	ebx;
  t_uint32	eax;
  t_uint8	reserved3[0x14];
  t_uint32	reserved2;
  t_uint32	ebp;
  t_uint32	eip;
  t_uint32	cs;
  t_uint32	eflags;
}		__attribute__ ((packed)) t_gdb_context;


void			ia32_gp_handler(t_id			id,
					t_uint32		error_code)
{
  printf("CPU%u: general protection fault @ %p\n", apic_id(), context->eip);

  while (1)
    ;
}

void			ia32_df_handler(t_id			id,
					t_uint32		error_code)
{
  printf("double fault.\n");

  while (1)
    ;
}

void			ia32_ts_handler(t_id			id,
					t_uint32		error_code)
{
  printf("invalid TSS.\n");

  while (1)
    ;
}

void                    ia32_pf_handler(t_id			id,
					t_uint32		error_code)
{
  t_uint32              addr;

  /*
   * 1)
   */

  SCR2(addr);

  printf("CPU%u: page fault\n"
         "  %p trying to %s at the address 0x%x requires %s\n",
	 apic_id(),
	 context->eip,
         (error_code & 2) ? "write" : "read",
         addr,
         (error_code & 1) ? "a lower DPL" : "the page to be present");

  while (1)
    ;
}

/*								[cut] /k2 */
