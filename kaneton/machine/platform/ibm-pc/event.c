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

#include <libc.h>

#include <platform/platform.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

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

t_error			ibmpc_pic_init(void)
{
  /*
   * 1)
   */

  OUTB(MASTER_PORT_A, MASTER_ICW1);
  OUTB(SLAVE_PORT_A, SLAVE_ICW1);

  /*
   * 2)
   */

  OUTB(MASTER_PORT_B, MASTER_ICW2);
  OUTB(SLAVE_PORT_B, SLAVE_ICW2);

  /*
   * 3)
   */

  OUTB(MASTER_PORT_B, MASTER_ICW3);
  OUTB(SLAVE_PORT_B, SLAVE_ICW3);

  /*
   * 4)
   */

  OUTB(MASTER_PORT_B, MASTER_ICW4);
  OUTB(SLAVE_PORT_B, SLAVE_ICW4);

  /*
   * 5)
   */

  OUTB(MASTER_PORT_B, 0xfb);
  OUTB(SLAVE_PORT_B, 0xff);

  return ERROR_NONE;
}

/*
 * turn on a given interrupt enabling its flag
 */

t_error			ibmpc_enable_irq(t_uint8		irq)
{
  t_uint8		mask;

  if (irq > 15)
    return ERROR_UNKNOWN;

  if (irq < 8)
    {
      INB(MASTER_PORT_B, mask);
      OUTB(MASTER_PORT_B, mask & ~(1 << irq));
    }
  else
    {
      INB(SLAVE_PORT_B, mask);
      OUTB(SLAVE_PORT_B, mask & ~(1 << (irq - 8)));
    }

  return ERROR_NONE;
}

/*
 * turn off a given interrupt
 */

t_error			ibmpc_disable_irq(t_uint8		irq)
{
  t_uint8		mask;

  if (irq > 15)
    return ERROR_UNKNOWN;

  if (irq < 8)
    {
      INB(MASTER_PORT_B, mask);
      OUTB(MASTER_PORT_B, mask | (1 << irq));
    }
  else
    {
      INB(SLAVE_PORT_B, mask);
      OUTB(SLAVE_PORT_B, mask | (1 << (irq - 8)));
    }

  return ERROR_NONE;
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
    return ERROR_UNKNOWN;

  /*
   * 2)
   */

  if (irq < 8)
    {
      INB(MASTER_PORT_B, mask);
      OUTB(MASTER_PORT_B, mask);
      OUTB(MASTER_PORT_A, 0x60 + irq);
    }
  else
    {
      INB(SLAVE_PORT_B, mask);
      OUTB(SLAVE_PORT_B, mask);
      OUTB(SLAVE_PORT_A, 0x60 + (irq & 0x7));
      OUTB(MASTER_PORT_A, 0x60 + 2);
    }

  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
