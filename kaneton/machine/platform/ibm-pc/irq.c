/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/platform/ibm-pc/event.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:44:58 2005]
 * updated       matthieu bucchianeri   [fri jun 15 01:32:33 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the management of the programmable interrupt
 * controller (PIC 8259A) on ia32 architecture ibmpc.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <platform/platform.h>
#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * initialize the pics
 *
 * steps:
 *
 * 1) send ICW1
 * 2) send ICW2
 * 3) send ICW3
 * 4) send ICW4
 * 5) all interrupts are set off at initialization
 *
 */

t_error			ibmpc_irq_init(void)
{
  /*
   * 1)
   */

  ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_A, IBMPC_IRQ_MASTER_ICW1);
  ARCHITECTURE_OUT_8(IBMPC_IRQ_SLAVE_PORT_A, IBMPC_IRQ_SLAVE_ICW1);

  /*
   * 2)
   */

  ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_B, IBMPC_IRQ_MASTER_ICW2);
  ARCHITECTURE_OUT_8(IBMPC_IRQ_SLAVE_PORT_B, IBMPC_IRQ_SLAVE_ICW2);

  /*
   * 3)
   */

  ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_B, IBMPC_IRQ_MASTER_ICW3);
  ARCHITECTURE_OUT_8(IBMPC_IRQ_SLAVE_PORT_B, IBMPC_IRQ_SLAVE_ICW3);

  /*
   * 4)
   */

  ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_B, IBMPC_IRQ_MASTER_ICW4);
  ARCHITECTURE_OUT_8(IBMPC_IRQ_SLAVE_PORT_B, IBMPC_IRQ_SLAVE_ICW4);

  /*
   * 5)
   */

  ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_B, 0xfb);
  ARCHITECTURE_OUT_8(IBMPC_IRQ_SLAVE_PORT_B, 0xff);

  return (ERROR_OK);
}

/*
 * turn on a given interrupt enabling its flag
 */

t_error			ibmpc_irq_enable(t_uint8		irq)
{
  t_uint8		mask;

  if (irq > 15)
    return (ERROR_KO);

  if (irq < 8)
    {
      ARCHITECTURE_IN_8(IBMPC_IRQ_MASTER_PORT_B, mask);
      ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_B, mask & ~(1 << irq));
    }
  else
    {
      ARCHITECTURE_IN_8(IBMPC_IRQ_SLAVE_PORT_B, mask);
      ARCHITECTURE_OUT_8(IBMPC_IRQ_SLAVE_PORT_B, mask & ~(1 << (irq - 8)));
    }

  return (ERROR_OK);
}

/*
 * turn off a given interrupt
 */

t_error			ibmpc_irq_disable(t_uint8		irq)
{
  t_uint8		mask;

  if (irq > 15)
    return (ERROR_KO);

  if (irq < 8)
    {
      ARCHITECTURE_IN_8(IBMPC_IRQ_MASTER_PORT_B, mask);
      ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_B, mask | (1 << irq));
    }
  else
    {
      ARCHITECTURE_IN_8(IBMPC_IRQ_SLAVE_PORT_B, mask);
      ARCHITECTURE_OUT_8(IBMPC_IRQ_SLAVE_PORT_B, mask | (1 << (irq - 8)));
    }

  return (ERROR_OK);
}

/*
 * acknowlegde the pics
 *
 * steps:
 *
 * 1) check irq bounds
 * 2) send normal EOI
 */

t_error			ibmpc_irq_acknowledge(t_uint8		irq)
{
  t_uint8		mask;

  /*
   * 1)
   */

  if (irq > 15)
    return (ERROR_KO);

  /*
   * 2)
   */

  if (irq < 8)
    {
      ARCHITECTURE_IN_8(IBMPC_IRQ_MASTER_PORT_B, mask);
      ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_B, mask);
      ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_A, 0x60 + irq);
    }
  else
    {
      ARCHITECTURE_IN_8(IBMPC_IRQ_SLAVE_PORT_B, mask);
      ARCHITECTURE_OUT_8(IBMPC_IRQ_SLAVE_PORT_B, mask);
      ARCHITECTURE_OUT_8(IBMPC_IRQ_SLAVE_PORT_A, 0x60 + (irq - 8));
      ARCHITECTURE_OUT_8(IBMPC_IRQ_MASTER_PORT_A, 0x60 + 2);
    }

  return (ERROR_OK);
}

