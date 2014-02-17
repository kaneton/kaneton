/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/machine/platform/ibm-pc/pic.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:44:58 2005]
 * updated       julien quintard   [fri jan 14 20:10:47 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the management of the PIC - Programmable Interrupt
 * Controller 8259A on ibmpc.
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
 * turn on the given interrupt line by enabling its flag in the appropriate
 * PIC i.e master or slave.
 *
 * steps:
 *
 * 1) check IRQ bounds.
 * 2) enable to flag associated with the given IRQ.
 */

t_status		platform_pic_enable(t_uint8		irq)
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
      ARCHITECTURE_IO_IN_8(PLATFORM_PIC_MASTER_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_B,
			    mask & ~(1 << irq));
    }
  else
    {
      ARCHITECTURE_IO_IN_8(PLATFORM_PIC_SLAVE_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_SLAVE_PORT_B,
			    mask & ~(1 << (irq - 8)));
    }

  MACHINE_LEAVE();
}

/*
 * turn off the given interrupt following the process for enabling IRQs.
 *
 * steps:
 *
 * 1) check IRQ bounds.
 * 2) disable to flag associated with the given IRQ.
 */

t_status		platform_pic_disable(t_uint8		irq)
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
      ARCHITECTURE_IO_IN_8(PLATFORM_PIC_MASTER_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_B,
			    mask | (1 << irq));
    }
  else
    {
      ARCHITECTURE_IO_IN_8(PLATFORM_PIC_SLAVE_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_SLAVE_PORT_B,
			    mask | (1 << (irq - 8)));
    }

  MACHINE_LEAVE();
}

/*
 * acknowlegde the appropriate PIC i.e master or slave for the given IRQ.
 *
 * steps:
 *
 * 1) check IRQ bounds.
 * 2) send the EOI - End Of Interrupt signal to the proper PIC.
 */

t_status		platform_pic_acknowledge(t_uint8	irq)
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
      ARCHITECTURE_IO_IN_8(PLATFORM_PIC_MASTER_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_B,
			    mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_A,
			    0x60 + irq);
    }
  else
    {
      ARCHITECTURE_IO_IN_8(PLATFORM_PIC_SLAVE_PORT_B,
			   mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_SLAVE_PORT_B,
			    mask);

      ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_SLAVE_PORT_A,
			    0x60 + (irq - 8));

      ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_A,
			    0x60 + 2);
    }

  MACHINE_LEAVE();
}

/*
 * initialize the PIC manager.
 *
 * steps:
 *
 * 1) send ICW1.
 * 2) send ICW2.
 * 3) send ICW3.
 * 4) send ICW4.
 * 5) all interrupts are set off at initialization.
 *
 */

t_status		platform_pic_initialize(void)
{
  /*
   * 1)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_A,
			PLATFORM_PIC_MASTER_ICW_1);

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_SLAVE_PORT_A,
			PLATFORM_PIC_SLAVE_ICW_1);

  /*
   * 2)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_B,
			PLATFORM_PIC_MASTER_ICW_2);

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_SLAVE_PORT_B,
			PLATFORM_PIC_SLAVE_ICW_2);

  /*
   * 3)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_B,
			PLATFORM_PIC_MASTER_ICW_3);

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_SLAVE_PORT_B,
			PLATFORM_PIC_SLAVE_ICW_3);

  /*
   * 4)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_B,
			PLATFORM_PIC_MASTER_ICW_4);

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_SLAVE_PORT_B,
			PLATFORM_PIC_SLAVE_ICW_4);

  /*
   * 5)
   */

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_MASTER_PORT_B,
			0xfb);

  ARCHITECTURE_IO_OUT_8(PLATFORM_PIC_SLAVE_PORT_B,
			0xff);

  MACHINE_LEAVE();
}

/*
 * clean the PIC manager.
 */

t_status		platform_pic_clean(void)
{
  MACHINE_LEAVE();
}
