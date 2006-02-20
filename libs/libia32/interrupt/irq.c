/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/interrupt/handlers.c
 *
 * created       renaud voltz   [fri feb 17 15:18:57 2006]
 * updated       renaud voltz   [fri feb 17 15:18:57 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements irq handling on ia32 architecture.
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
 * ---------- globals ---------------------------------------------------------
 */

/*
 * global irq handler table
 */

t_interrupt_handler		irq_handlers[16];

/*
 * ---------- functions -------------------------------------------------------
 */

static t_interrupt_handler	irq[16] =
  {
    handler_irq0,
    handler_irq1,
    handler_irq2,
    handler_irq3,
    handler_irq4,
    handler_irq5,
    handler_irq6,
    handler_irq7,
    handler_irq8,
    handler_irq9,
    handler_irq10,
    handler_irq11,
    handler_irq12,
    handler_irq13,
    handler_irq14,
    handler_irq15
  };

/*
 * add an idt entry
 *
 * steps:
 *
 * 1) check irq identifier
 * 2) set interrupt gate and add it into the idt
 */

t_error			irq_add(t_uint8				nr,
				t_prvl				privilege,
				t_interrupt_handler		handler)
{
  t_gate		gate;

  /*
   * 1)
   */

  if (nr > 15)
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  gate.offset = (t_uint32)irq[nr];
  gate.segsel = PMODE_GDT_CORE_CS << 3;
  gate.privilege = privilege;
  gate.type = type_gate_interrupt;

  idt_add_gate(NULL, IDT_IRQ_BASE + nr, gate);

  return ERROR_NONE;
}

/*
 * init irq with default handler
 *
 * steps:
 *
 * for each irq:
 * 1) assign default handler
 * 2) fill appropriate idt entry
 */

t_error			irq_init(void)
{
  int			i;

  for (i = 0; i < 16; i++)
    {

      /*
       * 1)
       */

      irq_handlers[i] = irq_default_handler;

      /*
       * 2)
       */

      if (irq_add(i, 0, irq[i]) != ERROR_NONE)
	return ERROR_UNKNOWN;
    }

  return ERROR_NONE;
}

/*
 * handle an irq
 *
 * steps:
 *
 * 1) disable maskable interrupts
 * 2) acknowledge the pic
 * 3) call interrupt handler
 * 4) enable maskable interrupts
 */

void			irq_wrapper(t_uint32			nr)
{
  CLI();

  pic_acknowledge(nr);

  irq_handlers[nr]();

  STI();
}

/*
 * do nothing
 */

void			irq_default_handler(void)
{
  printf("default irq handler\n");
}

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

