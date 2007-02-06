/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/event.c
 *
 * created       renaud voltz   [mon feb 13 01:05:52 2006]
 * updated       matthieu bucchianeri   [tue feb  6 01:40:49 2007]
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

#include <klibc.h>
#include <kaneton.h>

/*								    [cut] k3 */

/*
 * ---------- extern ---------------------------------------------------------
 */

/*
 * the kernel address space.
 */

extern i_as			kasid;

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

t_error			ia32_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler)
{
  t_uint32		eventid = id;

  /*
   * 1)
   */

  if (eventid >= EXCEPTION_NR + IRQ_NR)
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

  if (eventid >= EXCEPTION_NR + IRQ_NR)
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
 * 1) allocate space for the idt.
 * 2) build the idt and activate it.
 * 3) init events.
 * 4) disable writing the IDT.
 * 5) set default handler for every exception.
 * 6) set default handler for every irq.
 * 7) enable external interrupts.
 * 8) setup default exception handlers.
 */

t_error			ia32_event_init(void)
{
  int			id;
  t_vaddr		vaddr;
  t_ia32_idt		new_idt;
  o_region*		oreg;

  /*
   * 1)
   */

  if (map_reserve(kasid,
		  MAP_OPT_PRIVILEGED,
		  PAGESZ,
		  PERM_READ | PERM_WRITE,
		  &vaddr)
      != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (idt_build(IDT_MAX_ENTRIES, vaddr, 1, &new_idt) != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (idt_activate(&new_idt) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  if (interrupt_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 4)
   */

  if (region_get(kasid, (i_region)vaddr, &oreg) != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (segment_perms(oreg->segid, PERM_READ) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 5)
   */

  for (id = IDT_EXCEPTION_BASE; id < IDT_EXCEPTION_BASE + EXCEPTION_NR; id++)
    if (interrupt_set_handler(id, (t_ia32_interrupt_handler)
			      ia32_event_handler) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 6)
   */

  for (id = IDT_IRQ_BASE; id < IDT_IRQ_BASE + IRQ_NR; id++)
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
  printf("general protection fault @ %p\n", context->eip);

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

  t_gdb_context*	ctx;
  t_uint8*		ptr;

  /*
   * 1)
   */

  SCR2(addr);
  printf("#PF @ %p for %p\n", context->eip, addr);

  while (1)
    ;

  asm volatile("movl (%%ebp), %%eax\n\t"
	       "movl (%%eax), %%ebx\n\t"
	       "movl %%ebx, %0"
	       : "=g" (ptr)
	       :
	       : "%eax", "%ebx");

  ptr -= 56;
  ctx = (t_gdb_context*)ptr;

  SCR2(addr);

  printf("error: page fault !\n"
         "  %p trying to %s at the address 0x%x requires %s\n",
	 ctx->eip,
         (error_code & 2) ? "write" : "read",
         addr,
         (error_code & 1) ? "a lower DPL" : "the page to be present");

  t_ia32_directory pd;
  t_ia32_table pt;
  t_ia32_page pg;

  pd = ENTRY_ADDR(PD_MIRROR, PD_MIRROR);

  if (pd_get_table(&pd, PDE_ENTRY(addr), &pt) != ERROR_NONE)
    printf("no pde\n");
  else
    {
      printf("pde: %p\n", pt.entries);
      pt.entries = ENTRY_ADDR(PD_MIRROR, PDE_ENTRY(addr));

      if (pt_get_page(&pt, PTE_ENTRY(addr), &pg) != ERROR_NONE)
	printf("no pte\n");
      else
	{
	  printf("pte: %p\n", pg.addr);
	}
    }

  while (1);
}

/*								[cut] /k3 */
