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

#include <klibc.h>
#include <kaneton.h>

/*                                                                  [cut] k2 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * global interrupt handler table
 */

t_ia32_interrupt_handler	interrupt_handlers[EXCEPTION_NR + IRQ_NR + IPI_NR + SYSCALL_NR];

/*
 * the segment selector to load on interrupt.
 */

volatile HANDLER_DATA t_uint16		interrupt_ds = 0;

/*
 * the page directory to load on interrupt.
 */

volatile HANDLER_DATA t_uint32		interrupt_pdbr = 0;

volatile HANDLER_DATA t_uint32		interrupt_stack = 0;

/*
 * ---------- functions -------------------------------------------------------
 */

static t_ia32_interrupt_prehandler	prehandlers[EXCEPTION_NR + IRQ_NR + IPI_NR + SYSCALL_NR] =
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
    prehandler_syscall1
  };

/*
 * add an idt entry.
 *
 * steps:
 *
 * 1) check interrupt identifier.
 * 3) build an interrupt gate and add it into the idt.
 */

t_error			interrupt_add(t_uint32			nr,
				      t_ia32_prvl	       	privilege,
				      t_ia32_interrupt_prehandler prehandler)
{
  t_ia32_gate		gate;

  /*
   * 1)
   */

  if (nr >= EXCEPTION_NR + IRQ_NR + IPI_NR + SYSCALL_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  gate.offset = (t_uint32)prehandler;
  gate.segsel = PMODE_GDT_CORE_CS << 3;
  gate.privilege = privilege;
  gate.type = ia32_type_gate_interrupt;

  return idt_add_gate(NULL, nr, gate);
}

/*
 * set the interrupt generic handler.
 */

t_error			interrupt_set_handler(t_uint32			nr,
					      t_ia32_interrupt_handler	handler)
{
  interrupt_handlers[nr] = handler;

  return ERROR_NONE;
}

/*
 * init interrupts.
 *
 * steps:
 *
 * 1) add an interrupt gate descriptor in the idt for each exception.
 * 2) add an interrupt gate descriptor in the idt for each irq.
 * 3) add an interrupt gate descriptor in the idt for each ipi.
 * 3) add an interrupt gate descriptor in the idt for each syscall.
 * 4) initialize the pic 8259A.
 */

t_error			interrupt_init(void)
{
  int			i;

  /*
   * 1)
   */

  for (i = IDT_EXCEPTION_BASE; i < IDT_EXCEPTION_BASE + EXCEPTION_NR; i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 2)
   */

  for (i = IDT_IRQ_BASE; i < IDT_IRQ_BASE + IRQ_NR; i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 3)
   */

  for (i = IDT_IPI_BASE; i < IDT_IPI_BASE + IPI_NR; i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 4)
   */

  for (i = IDT_SYSCALL_BASE; i < IDT_SYSCALL_BASE + SYSCALL_NR; i++)
    if (interrupt_add(i, 0, prehandlers[i]) != ERROR_NONE)
      return ERROR_UNKNOWN;

  /*
   * 5)
   */

  if (pic_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * this is the second handler called on exception.
 * it calls the user-defined handler.
 */

void			handler_exception(t_uint32			nr,
					  t_uint32			code)
{
  interrupt_handlers[nr](nr, code);
}

/*
 * this is the second handler called on irq.  it sends the EOI and
 * call the user-defined handler.
 */

void			handler_irq(t_uint32				nr)
{
  pic_acknowledge(nr);

  interrupt_handlers[IDT_IRQ_BASE + nr](IDT_IRQ_BASE + nr, 0);
}

/*
 * this is the second handler called on ipi.  it sends the EOI and
 * call the user-defined handler.
 */

void			handler_ipi(t_uint32				nr)
{
  ipi_acknowledge();

  interrupt_handlers[IDT_IPI_BASE + nr](IDT_IPI_BASE + nr, 0);
}

/*
 * this is the second handler called on syscall.  it calls the
 * user-defined handler.
 */

void			handler_syscall(t_uint32			nr)
{
  interrupt_handlers[IDT_SYSCALL_BASE + nr](IDT_SYSCALL_BASE + nr, 0);
}

/*
 * exception pre-handlers definitions
 */

EXCEPTION_PREHANDLER_NOCODE(0);
EXCEPTION_PREHANDLER_NOCODE(1);
EXCEPTION_PREHANDLER_NOCODE(2);
EXCEPTION_PREHANDLER_NOCODE(3);
EXCEPTION_PREHANDLER_NOCODE(4);
EXCEPTION_PREHANDLER_NOCODE(5);
EXCEPTION_PREHANDLER_NOCODE(6);
EXCEPTION_PREHANDLER_NOCODE(7);
EXCEPTION_PREHANDLER_CODE(8);
EXCEPTION_PREHANDLER_NOCODE(9);
EXCEPTION_PREHANDLER_CODE(10);
EXCEPTION_PREHANDLER_CODE(11);
EXCEPTION_PREHANDLER_CODE(12);
EXCEPTION_PREHANDLER_CODE(13);
EXCEPTION_PREHANDLER_CODE(14);
EXCEPTION_PREHANDLER_NOCODE(15);
EXCEPTION_PREHANDLER_NOCODE(16);
EXCEPTION_PREHANDLER_CODE(17);
EXCEPTION_PREHANDLER_NOCODE(18);
EXCEPTION_PREHANDLER_NOCODE(19);
EXCEPTION_PREHANDLER_NOCODE(20);
EXCEPTION_PREHANDLER_NOCODE(21);
EXCEPTION_PREHANDLER_NOCODE(22);
EXCEPTION_PREHANDLER_NOCODE(23);
EXCEPTION_PREHANDLER_NOCODE(24);
EXCEPTION_PREHANDLER_NOCODE(25);
EXCEPTION_PREHANDLER_NOCODE(26);
EXCEPTION_PREHANDLER_NOCODE(27);
EXCEPTION_PREHANDLER_NOCODE(28);
EXCEPTION_PREHANDLER_NOCODE(29);
EXCEPTION_PREHANDLER_NOCODE(30);
EXCEPTION_PREHANDLER_NOCODE(31);

/*
 * irq pre-handlers definitions
 */

IRQ_PREHANDLER(0);
IRQ_PREHANDLER(1);
IRQ_PREHANDLER(2);
IRQ_PREHANDLER(3);
IRQ_PREHANDLER(4);
IRQ_PREHANDLER(5);
IRQ_PREHANDLER(6);
IRQ_PREHANDLER(7);
IRQ_PREHANDLER(8);
IRQ_PREHANDLER(9);
IRQ_PREHANDLER(10);
IRQ_PREHANDLER(11);
IRQ_PREHANDLER(12);
IRQ_PREHANDLER(13);
IRQ_PREHANDLER(14);
IRQ_PREHANDLER(15);

/*
 * ipi prehandlers
 */

IPI_PREHANDLER(0);
IPI_PREHANDLER(1);
IPI_PREHANDLER(2);
IPI_PREHANDLER(3);
IPI_PREHANDLER(4);
IPI_PREHANDLER(5);
IPI_PREHANDLER(6);
IPI_PREHANDLER(7);

/*
 * syscall prehandlers
 */

SYSCALL_PREHANDLER(0);
SYSCALL_PREHANDLER(1);

/*                                                                 [cut] /k2 */
