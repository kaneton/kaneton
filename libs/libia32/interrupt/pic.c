/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/interrupt/interrupt.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:44:58 2005]
 * updated       matthieu bucchianeri   [mon jan 30 23:49:40 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage interrupt controller.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students will use this file to place functions they wrote to manage
 * the pic. students may write as many functions as they whant without
 * any restrictions.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*                                                                  [cut] k3 */

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * pics addresses
 */

#define MASTER_PORT_A		0x20
#define MASTER_PORT_B		0x21
#define SLAVE_PORT_A		0xa0
#define SLAVE_PORT_B		0xa1

/*
 * master icw's
 */

#define MASTER_ICW1		0x11
#define MASTER_ICW2		0x20
#define MASTER_ICW3		0x04
#define MASTER_ICW4		0x01

/*
 * slave icw's
 */

#define SLAVE_ICW1		0x11
#define SLAVE_ICW2		0x28
#define SLAVE_ICW3		0x02
#define SLAVE_ICW4		0x01

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

t_error			interrupt_init(void)
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

  OUTB(MASTER_PORT_B, 0xff);
  OUTB(SLAVE_PORT_B, 0xff);

  return ERROR_NONE;
}

/*
 * turn on a given interrupt enabling its flag
 */

t_error			interrupt_enable(t_uint8		irq)
{
  t_uint8		mask;

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

t_error			interrupt_disable(t_uint8		irq)
{
  t_uint8		mask;

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

/*                                                                 [cut] /k3 */
