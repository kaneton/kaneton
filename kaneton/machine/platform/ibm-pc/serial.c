/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...kaneton/machine/platform/ibm-pc/serial.c
 *
 * created       julien quintard   [sat may 28 18:23:13 2005]
 * updated       julien quintard   [fri jan 14 20:13:12 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file provides functionalities for sending and receiving data
 * to/from a serial line.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <platform/platform.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function reads on the given serial device.
 *
 * steps:
 *
 * 1) until enough bytes have been read.
 *   a) read the status register.
 *   b) if the data is ready to be read, read the register.
 */

void        		platform_serial_read(t_uint32		port,
					     t_uint8*		data,
					     t_uint32 		size)
{
  t_uint8		status;

  /*
   * 1)
   */

  while (size)
    {
      /*
       * a)
       */

      ARCHITECTURE_IO_IN_8(port + 5,
			   status);

      /*
       * b)
       */

      if (status & 1)
	{
	  ARCHITECTURE_IO_IN_8(port,
			       *data++);

	  size--;
	}
    }
}

/*
 * this function writes on the serial device.
 *
 * 1) until enough bytes have been written.
 *   a) read the status register.
 *   b) if the register is ready to be written, write it.
 */

void			platform_serial_write(t_uint32		port,
					      t_uint8*		data,
					      t_uint32		size)
{
  t_uint8		status;

  /*
   * 1)
   */

  while(size)
    {
      /*
       * a)
       */

      ARCHITECTURE_IO_IN_8(port + 5,
			   status);

      /*
       * b)
       */

      if (status & 0x20)
	{
	  ARCHITECTURE_IO_OUT_8(port,
				*data++);

	  size--;
	}
    }
}

/*
 * this function sets up the serial port.
 *
 * steps:
 *
 * 1) disable IRQs.
 * 2) enable the baud rate divisor.
 * 3) set the baude rate divisor.
 * 4) set the serial type such as 8 bits, no parity and one bit stop which
 *    is referred to as 8N1.
 * 5) enable the FIFO.
 * 6) re-enable IRQs.
 */

t_status		platform_serial_setup(t_uint32		port,
					      t_uint8		rate,
					      t_uint8		type)
{
  /*
   * 1)
   */

  ARCHITECTURE_IO_OUT_8(port + 1,
			0x00);

  /*
   * 2)
   */

  ARCHITECTURE_IO_OUT_8(port + 3,
			0x80);

  /*
   * 3)
   */

  ARCHITECTURE_IO_OUT_8(port + 0,
			rate);

  ARCHITECTURE_IO_OUT_8(port + 1,
			0x00);

  /*
   * 4)
   */

  ARCHITECTURE_IO_OUT_8(port + 3,
			type);

  /*
   * 5)
   */

  ARCHITECTURE_IO_OUT_8(port + 2,
			PLATFORM_SERIAL_FIFO_8);

  /*
   * 6)
   */

  ARCHITECTURE_IO_OUT_8(port + 4,
			0x08);

  MACHINE_LEAVE();
}

/*
 * this function initializes the serial manager.
 */

t_status		platform_serial_initialize(void)
{
  MACHINE_LEAVE();
}

/*
 * this function cleans the serial manager.
 */

t_status		platform_serial_clean(void)
{
  MACHINE_LEAVE();
}
