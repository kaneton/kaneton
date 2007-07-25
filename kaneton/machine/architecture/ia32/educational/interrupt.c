/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/interrupt/interrupt.c
 *
 * created       renaud voltz   [thu feb 23 10:49:43 2006]
 * updated       matthieu bucchianeri   [wed mar 21 22:54:16 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements exception and irq handling on ia32 architecture.
 * both exceptions and irq's are called interrupts in the machine-dependent
 * code of kaneton.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*                                                                  [cut] k2 */

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as	kasid;

/*
 * ---------- non-generated prototypes ----------------------------------------
 */

PREHANDLER_PROTOTYPES();

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment selector to load on interrupt.
 */

volatile IA32_HANDLER_DATA_SECTION t_uint16	ia32_interrupt_ds = 0;

/*
 * the page directory to load on interrupt.
 */

volatile IA32_HANDLER_DATA_SECTION t_uint32	ia32_interrupt_pdbr = 0;

volatile IA32_HANDLER_DATA_SECTION t_uint32	ia32_interrupt_stack = 0;

/*
 * ---------- functions -------------------------------------------------------
 */

static t_ia32_interrupt_prehandler		prehandlers[IA32_HANDLER_NR] =
  {
    prehandler_exception0,
    prehandler_exception1,
    prehandler_exception2,
    prehandler_exception3,
    prehandler_exception4,
    prehandler_exception5,
    prehandler_exception6,
    prehandler_exception7,
    prehandler_exception8,
    prehandler_exception9,
    prehandler_exception10,
    prehandler_exception11,
    prehandler_exception12,
    prehandler_exception13,
    prehandler_exception14,
    prehandler_exception15,
    prehandler_exception16,
    prehandler_exception17,
    prehandler_exception18,
    prehandler_exception19,
    prehandler_exception20,
    prehandler_exception21,
    prehandler_exception22,
    prehandler_exception23,
    prehandler_exception24,
    prehandler_exception25,
    prehandler_exception26,
    prehandler_exception27,
    prehandler_exception28,
    prehandler_exception29,
    prehandler_exception30,
    prehandler_exception31,
    prehandler_irq0,
    prehandler_irq1,
    prehandler_irq2,
    prehandler_irq3,
    prehandler_irq4,
    prehandler_irq5,
    prehandler_irq6,
    prehandler_irq7,
    prehandler_irq8,
    prehandler_irq9,
    prehandler_irq10,
    prehandler_irq11,
    prehandler_irq12,
    prehandler_irq13,
    prehandler_irq14,
    prehandler_irq15,
    prehandler_ipi0,
    prehandler_ipi1,
    prehandler_ipi2,
    prehandler_ipi3,
    prehandler_ipi4,
    prehandler_ipi5,
    prehandler_ipi6,
    prehandler_ipi7,
    prehandler_syscall0,
    prehandler_syscall1,
    prehandler_syscall2,
    prehandler_syscall3,
    prehandler_syscall4,
    prehandler_syscall5,
    prehandler_syscall6,
    prehandler_syscall7,
    prehandler_syscall8
  };

/*
 * add an idt entry.
 *
 * steps:
 *
 * 1) check interrupt identifier.
 * 3) build an interrupt gate and add it into the idt.
 */

static t_error		interrupt_add(t_uint32			nr,
				      t_ia32_prvl	       	privilege,
				      t_ia32_interrupt_prehandler prehandler)
{
  t_ia32_gate		gate;

  /*
   * 1)
   */

  if (nr >= IA32_HANDLER_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  gate.offset = (t_uint32)prehandler;
  gate.segsel = IA32_PMODE_GDT_CORE_CS << 3;
  gate.privilege = privilege;
  gate.type = ia32_type_gate_interrupt;

  return ia32_idt_add_gate(NULL, nr, gate);
}

/*
 * init interrupts.
 *
 * steps:
 *
 * 1) allocate space for the idt.
 * 2) build the idt and activate it.
 * 3) add an interrupt gate descriptor in the idt for each exception.
 * 4) add an interrupt gate descriptor in the idt for each irq.
 * 5) add an interrupt gate descriptor in the idt for each ipi.
 * 6) add an interrupt gate descriptor in the idt for each syscall.
 */

t_error			ia32_interrupt_vector_init(void)
{
  int			i;
  t_vaddr		vaddr;
  t_ia32_idt		new_idt;

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

  if (ia32_idt_build(IA32_IDT_MAX_ENTRIES, vaddr, 1, &new_idt) != ERROR_NONE)
    return ERROR_UNKNOWN;

  if (ia32_idt_activate(&new_idt) != ERROR_NONE)
    return ERROR_UNKNOWN;

  /*
   * 3)
   */

  for (i = IA32_IDT_EXCEPTION_BASE;
       i < IA32_IDT_EXCEPTION_BASE + IA32_EXCEPTION_NR;
       i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 4)
   */

  for (i = IA32_IDT_IRQ_BASE;
       i < IA32_IDT_IRQ_BASE + IA32_IRQ_NR;
       i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 5)
   */

  for (i = IA32_IDT_IPI_BASE;
       i < IA32_IDT_IPI_BASE + IA32_IPI_NR;
       i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 6)
   */

  for (i = IA32_IDT_SYSCALL_BASE;
       i < IA32_IDT_SYSCALL_BASE + IA32_SYSCALL_NR;
       i++)
    if (interrupt_add(i, 3, prehandlers[i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * this handler is called on unhandled interrupts.
 */

static void		spurious_interrupt(i_event			id)
{
  printf("Unhandled exception %qu @ %p\n", id, ia32_context->eip);

  while (1)
    ;
}

/*
 * this is the second handler called on exception.
 * it calls the user-defined handler.
 */

void			ia32_handler_exception(t_uint32			nr,
					       t_uint32			code)
{
  o_event*		o;
  i_event		id = nr;

  if (event_get(id, &o) == ERROR_NONE)
    {
      if (o->type == EVENT_FUNCTION)
	IA32_CALL_HANDLER(o->handler, id, code);
      else
	event_notify(id);
    }
  else
    {
      spurious_interrupt(id);
    }
}

/*
 * this is the second handler called on irq.  it sends the EOI and
 * call the user-defined handler.
 */

void			ia32_handler_irq(t_uint32			nr)
{
  o_event*		o;
  i_event		id = IA32_IDT_IRQ_BASE + nr;
  i_thread		current;

  ASSERT(view_signal("irq", nr, VIEW_SIGNAL_IRQ) == ERROR_NONE);

  // XXX pic ack
  if (nr >= 8)
    OUTB(0xA0, 0x20);
  OUTB(0x20, 0x20);

  if (event_get(id, &o) == ERROR_NONE)
    {
      if (o->type == EVENT_FUNCTION)
	IA32_CALL_HANDLER(o->handler, id, 0);
      else
	event_notify(id);
    }
  else
    {
      spurious_interrupt(id);
    }

  ASSERT(scheduler_current(&current) == ERROR_NONE);
  ASSERT(view_signal("thread", current, VIEW_SIGNAL_RESUME) == ERROR_NONE);
}

/*
 * this is the second handler called on ipi.  it sends the EOI and
 * call the user-defined handler.
 */

void			ia32_handler_ipi(t_uint32			nr)
{
  o_event*		o;
  i_event		id = IA32_IDT_IPI_BASE + nr;

  ia32_ipi_acknowledge();

  if (event_get(id, &o) == ERROR_NONE)
    {
      if (o->type == EVENT_FUNCTION)
	IA32_CALL_HANDLER(o->handler, id, 0);
      else
	event_notify(id);
    }
  else
    {
      spurious_interrupt(id);
    }
}

/*
 * this is the second handler called on syscall.  it calls the
 * user-defined handler.
 */

void			ia32_handler_syscall(t_uint32			nr)
{
  o_event*		o;
  i_event		id = IA32_IDT_SYSCALL_BASE + nr;
  i_thread		current;

  ASSERT(view_signal("syscall", nr, VIEW_SIGNAL_SYSCALL) == ERROR_NONE);

  if (event_get(id, &o) == ERROR_NONE)
    {
      if (o->type == EVENT_FUNCTION)
	IA32_CALL_HANDLER(o->handler, id, 0);
      else
	event_notify(id);
    }
  else
    {
      spurious_interrupt(id);
    }

  ASSERT(scheduler_current(&current) == ERROR_NONE);
  ASSERT(view_signal("thread", current, VIEW_SIGNAL_RESUME) == ERROR_NONE);
}

/*
 * exception pre-handlers definitions
 */

IA32_EXCEPTION_PREHANDLER_NOCODE(0);
IA32_EXCEPTION_PREHANDLER_NOCODE(1);
IA32_EXCEPTION_PREHANDLER_NOCODE(2);
IA32_EXCEPTION_PREHANDLER_NOCODE(3);
IA32_EXCEPTION_PREHANDLER_NOCODE(4);
IA32_EXCEPTION_PREHANDLER_NOCODE(5);
IA32_EXCEPTION_PREHANDLER_NOCODE(6);
IA32_EXCEPTION_PREHANDLER_NOCODE(7);
IA32_EXCEPTION_PREHANDLER_CODE(8);
IA32_EXCEPTION_PREHANDLER_NOCODE(9);
IA32_EXCEPTION_PREHANDLER_CODE(10);
IA32_EXCEPTION_PREHANDLER_CODE(11);
IA32_EXCEPTION_PREHANDLER_CODE(12);
IA32_EXCEPTION_PREHANDLER_CODE(13);
IA32_EXCEPTION_PREHANDLER_CODE(14);
IA32_EXCEPTION_PREHANDLER_NOCODE(15);
IA32_EXCEPTION_PREHANDLER_NOCODE(16);
IA32_EXCEPTION_PREHANDLER_CODE(17);
IA32_EXCEPTION_PREHANDLER_NOCODE(18);
IA32_EXCEPTION_PREHANDLER_NOCODE(19);
IA32_EXCEPTION_PREHANDLER_NOCODE(20);
IA32_EXCEPTION_PREHANDLER_NOCODE(21);
IA32_EXCEPTION_PREHANDLER_NOCODE(22);
IA32_EXCEPTION_PREHANDLER_NOCODE(23);
IA32_EXCEPTION_PREHANDLER_NOCODE(24);
IA32_EXCEPTION_PREHANDLER_NOCODE(25);
IA32_EXCEPTION_PREHANDLER_NOCODE(26);
IA32_EXCEPTION_PREHANDLER_NOCODE(27);
IA32_EXCEPTION_PREHANDLER_NOCODE(28);
IA32_EXCEPTION_PREHANDLER_NOCODE(29);
IA32_EXCEPTION_PREHANDLER_NOCODE(30);
IA32_EXCEPTION_PREHANDLER_NOCODE(31);

/*
 * irq pre-handlers definitions
 */

IA32_IRQ_PREHANDLER(0);
IA32_IRQ_PREHANDLER(1);
IA32_IRQ_PREHANDLER(2);
IA32_IRQ_PREHANDLER(3);
IA32_IRQ_PREHANDLER(4);
IA32_IRQ_PREHANDLER(5);
IA32_IRQ_PREHANDLER(6);
IA32_IRQ_PREHANDLER(7);
IA32_IRQ_PREHANDLER(8);
IA32_IRQ_PREHANDLER(9);
IA32_IRQ_PREHANDLER(10);
IA32_IRQ_PREHANDLER(11);
IA32_IRQ_PREHANDLER(12);
IA32_IRQ_PREHANDLER(13);
IA32_IRQ_PREHANDLER(14);
IA32_IRQ_PREHANDLER(15);

/*
 * ipi prehandlers
 */

IA32_IPI_PREHANDLER(0);
IA32_IPI_PREHANDLER(1);
IA32_IPI_PREHANDLER(2);
IA32_IPI_PREHANDLER(3);
IA32_IPI_PREHANDLER(4);
IA32_IPI_PREHANDLER(5);
IA32_IPI_PREHANDLER(6);
IA32_IPI_PREHANDLER(7);

/*
 * syscall prehandlers
 */

IA32_SYSCALL_PREHANDLER(0);
IA32_SYSCALL_PREHANDLER(1);
IA32_SYSCALL_PREHANDLER(2);
IA32_SYSCALL_PREHANDLER(3);
IA32_SYSCALL_PREHANDLER(4);
IA32_SYSCALL_PREHANDLER(5);
IA32_SYSCALL_PREHANDLER(6);
IA32_SYSCALL_PREHANDLER(7);
IA32_SYSCALL_PREHANDLER(8);

/*                                                                 [cut] /k2 */
/*
 * send a INIT IPI.
 */

void			ia32_ipi_send_init(void)
{
  ia32_apic_write(IA32_APIC_REG_ICR_HI, 0);
  ia32_apic_write(IA32_APIC_REG_ICR_LOW, 0xC4500);
}

/*
 * send a SIPI.
 */

void			ia32_ipi_send_startup(void)
{
  ia32_apic_write(IA32_APIC_REG_ICR_HI, 0);
  ia32_apic_write(IA32_APIC_REG_ICR_LOW, 0xC4608);
}

/*
 * send a vector IPI.
 */

void			ia32_ipi_send_vector(t_uint8		vector,
					     t_ia32_ipi_dest	dest,
					     i_cpu		cpu)
{
  switch (dest)
    {
      case ipi_all:
	ia32_apic_write(IA32_APIC_REG_ICR_HI, 0);
	ia32_apic_write(IA32_APIC_REG_ICR_LOW, (1 << 19) | (1 << 14) | vector);
	break;
      case ipi_all_but_me:
	ia32_apic_write(IA32_APIC_REG_ICR_HI, 0);
	ia32_apic_write(IA32_APIC_REG_ICR_LOW, (1 << 19) | (1 << 18) | (1 << 14) |
		   vector);
	break;
      case ipi_cpu:
	ia32_apic_write(IA32_APIC_REG_ICR_HI, (t_uint8)cpu << 24);
	ia32_apic_write(IA32_APIC_REG_ICR_LOW, (1 << 14) | vector);
	break;
    }
}

/*
 * sends EOI.
 */

void			ia32_ipi_acknowledge(void)
{
  ia32_apic_write(IA32_APIC_REG_EOI, 0);
}

/*								[cut] /k2 */
