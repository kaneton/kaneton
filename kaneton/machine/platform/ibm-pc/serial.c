/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/debug/serial.c
 *
 * created       julien quintard   [sat may 28 18:23:13 2005]
 * updated       matthieu bucchianeri   [sun may 27 14:47:01 2007]
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
 * this function read on the serial device
 */

void        		platform_serial_read(t_uint32		com_port,
					     t_uint8*		data,
					     t_uint32 		size)
{

  while (size)
    {
      ARCHITECTURE_IO_IN_8(com_port + 5,
			   *data);

      if (*data & 1)
	{
	  ARCHITECTURE_IO_IN_8(com_port,
			       *data++);

	  size--;
	}
    }
}

/*
 * this function writes on the serial device
 */

void			platform_serial_write(t_uint32		com_port,
					      t_uint8*		data,
					      t_uint32		size)
{
  char			ch;

  while(size)
    {
      ARCHITECTURE_IO_IN_8(com_port + 5,
			   ch);

      if (ch & 0x20)
	{
	  ARCHITECTURE_IO_OUT_8(com_port,
				*data++);

	  size--;
	}
    }
}

/*
 * this function initializes the serial port.
 */

t_error			platform_serial_setup(t_uint32		com_port,
					      t_uint8		baud_rate,
					      t_uint8		bit_type)
{
  ARCHITECTURE_IO_OUT_8(com_port + 1,
			0x00);

  ARCHITECTURE_IO_OUT_8(com_port + 3,
			0x80);

  ARCHITECTURE_IO_OUT_8(com_port + 0,
			baud_rate);

  ARCHITECTURE_IO_OUT_8(com_port + 1,
			0x00);

  ARCHITECTURE_IO_OUT_8(com_port + 3,
			bit_type);

  ARCHITECTURE_IO_OUT_8(com_port + 2,
			PLATFORM_SERIAL_FIFO_8);

  ARCHITECTURE_IO_OUT_8(com_port + 4,
			0x08);

  MACHINE_LEAVE();
}
