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

#include <kaneton.h>

#include <platform/platform.h>
#include <architecture/architecture.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * turn on a given interrupt enabling its flag
 */

t_error			platform_irq_enable(t_uint8		irq)
{
  t_uint8		mask;

  if (irq > 15)
    MACHINE_ESCAPE("the irq number '%u' is invalid",
		   irq);

  if (irq < 8)
    {
      ARCHITECTURE_IO_IN_8(PLATFORM_IRQ_MASTER_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_B,
			    mask & ~(1 << irq));
    }
  else
    {
      ARCHITECTURE_IO_IN_8(PLATFORM_IRQ_SLAVE_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_SLAVE_PORT_B,
			    mask & ~(1 << (irq - 8)));
    }

  MACHINE_LEAVE();
}

/*
 * turn off a given interrupt
 */

t_error			platform_irq_disable(t_uint8		irq)
{
  t_uint8		mask;

  if (irq > 15)
    MACHINE_ESCAPE("the irq number '%u' is invalid",
		   irq);

  if (irq < 8)
    {
      ARCHITECTURE_IO_IN_8(PLATFORM_IRQ_MASTER_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_B,
			    mask | (1 << irq));
    }
  else
    {
      ARCHITECTURE_IO_IN_8(PLATFORM_IRQ_SLAVE_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_SLAVE_PORT_B,
			    mask | (1 << (irq - 8)));
    }

  MACHINE_LEAVE();
}

/*
 * acknowlegde the pics
 *
 * steps:
 *
 * 1) check irq bounds
 * 2) send normal EOI
 */

t_error			platform_irq_acknowledge(t_uint8	irq)
{
  t_uint8		mask;

  /*
   * 1)
   */

  if (irq > 15)
    MACHINE_ESCAPE("the irq number '%u' is invalid",
		   irq);

  /*
   * 2)
   */

  if (irq < 8)
    {
      ARCHITECTURE_IO_IN_8(PLATFORM_IRQ_MASTER_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_B,
			    mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_A,
			    0x60 + irq);
    }
  else
    {
      ARCHITECTURE_IO_IN_8(PLATFORM_IRQ_SLAVE_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_SLAVE_PORT_B,
			    mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_SLAVE_PORT_A,
			    0x60 + (irq - 8));

      ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_A,
			    0x60 + 2);
    }

  MACHINE_LEAVE();
}

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

t_error			platform_irq_initialize(void)
{
  /*
   * 1)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_A,
			PLATFORM_IRQ_MASTER_ICW_1);

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_SLAVE_PORT_A,
			PLATFORM_IRQ_SLAVE_ICW_1);

  /*
   * 2)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_B,
			PLATFORM_IRQ_MASTER_ICW_2);

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_SLAVE_PORT_B,
			PLATFORM_IRQ_SLAVE_ICW_2);

  /*
   * 3)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_B,
			PLATFORM_IRQ_MASTER_ICW_3);

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_SLAVE_PORT_B,
			PLATFORM_IRQ_SLAVE_ICW_3);

  /*
   * 4)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_B,
			PLATFORM_IRQ_MASTER_ICW_4);

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_SLAVE_PORT_B,
			PLATFORM_IRQ_SLAVE_ICW_4);

  /*
   * 5)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_MASTER_PORT_B,
			0xfb);

  ARCHITECTURE_IO_OUT_8(PLATFORM_IRQ_SLAVE_PORT_B,
			0xff);

  MACHINE_LEAVE();
}

/*
 * clean the irqs.
 */

t_error			platform_irq_clean(void)
{
  MACHINE_LEAVE();
}
