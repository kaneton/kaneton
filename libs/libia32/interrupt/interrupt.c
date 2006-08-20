/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/interrupt/interrupt.c
 *
 * created       renaud voltz   [thu feb 23 10:49:43 2006]
 * updated       matthieu bucchianeri   [thu aug 17 20:14:34 2006]
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

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * global interrupt handler table
 */

t_ia32_interrupt_handler	interrupt_handlers[EXCEPTION_NR + IRQ_NR];

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

static t_ia32_interrupt_prehandler	prehandlers[EXCEPTION_NR + IRQ_NR] =
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
    prehandler_irq15
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

  if (nr >= EXCEPTION_NR + IRQ_NR)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  gate.offset = (t_uint32)prehandler;
  gate.segsel = PMODE_GDT_CORE_CS << 3;
  gate.privilege = privilege;
  gate.type = ia32_type_gate_interrupt;

  idt_add_gate(NULL, nr, gate);

  return ERROR_NONE;
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
 * 3) initialize the pic 8259A.
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

  if (pic_init() != ERROR_NONE)
    return ERROR_UNKNOWN;

  return ERROR_NONE;
}

/*
 * this is the second handler called on exception.
 * it saves the context second part and call the user-defined handler.
 */

void			handler_exception(t_uint32			nr,
					  t_uint32			has_code)
{
  t_uint32		code = 0;

  SAVE_CONTEXT();

  RING0_SWITCH();

  if (has_code)
    {
      GET_ERROR_CODE(code);
    }

  interrupt_handlers[nr](nr, code);

  RING0_BACK();

  RESTORE_CONTEXT();
}

/*
 * this is  the second  handler called on  irq.  it saves  the context
 * second part, send the EOI and call the user-defined handler.
 */


void			handler_irq(t_uint32				nr)
{
  UNUSED t_uint32	code = 0;

  SAVE_CONTEXT();

  RING0_SWITCH();

  pic_acknowledge(nr - IDT_IRQ_BASE);

  interrupt_handlers[nr](nr, 0);

  RING0_BACK();

  RESTORE_CONTEXT();
}

/*
 * exception pre-handlers definitions
 */

EXCEPTION_PREHANDLER(0, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(1, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(2, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(3, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(4, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(5, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(6, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(7, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(8, ERROR_CODE)
EXCEPTION_PREHANDLER(9, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(10, ERROR_CODE)
EXCEPTION_PREHANDLER(11, ERROR_CODE)
EXCEPTION_PREHANDLER(12, ERROR_CODE)
EXCEPTION_PREHANDLER(13, ERROR_CODE)
EXCEPTION_PREHANDLER(14, ERROR_CODE)
EXCEPTION_PREHANDLER(15, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(16, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(17, ERROR_CODE)
EXCEPTION_PREHANDLER(18, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(19, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(20, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(21, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(22, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(23, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(24, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(25, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(26, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(27, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(28, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(29, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(30, NO_ERROR_CODE)
EXCEPTION_PREHANDLER(31, NO_ERROR_CODE)

/*
 * irq pre-handlers definitions
 */

IRQ_PREHANDLER(0)
IRQ_PREHANDLER(1)
IRQ_PREHANDLER(2)
IRQ_PREHANDLER(3)
IRQ_PREHANDLER(4)
IRQ_PREHANDLER(5)
IRQ_PREHANDLER(6)
IRQ_PREHANDLER(7)
IRQ_PREHANDLER(8)
IRQ_PREHANDLER(9)
IRQ_PREHANDLER(10)
IRQ_PREHANDLER(11)
IRQ_PREHANDLER(12)
IRQ_PREHANDLER(13)
IRQ_PREHANDLER(14)
IRQ_PREHANDLER(15)
