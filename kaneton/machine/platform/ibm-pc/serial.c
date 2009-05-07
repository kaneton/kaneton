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

void        		ibmpc_serial_read(t_uint32		com_port,
					  t_uint8*		data,
					  t_uint32 		size)
{

  while (size)
    {
      INB(com_port + 5, *data);
      if (*data & 1)
	{
	  INB(com_port, *data++);
	  size--;
	}
    }
}

/*
 * this function writes on the serial device
 */

void			ibmpc_serial_write(t_uint32		com_port,
					   t_uint8*		data,
					   t_uint32		size)
{
  char			ch;

  while(size)
    {
      INB(com_port + 5, ch);
      if (ch & 0x20)
	{
	  OUTB(com_port, *data++);
	  size--;
	}
    }
}

/*
 * this function initializes the serial port.
 */

t_error			ibmpc_serial_init(t_uint32			com_port,
				    t_uint8			baud_rate,
				    t_uint8			bit_type)
{
  OUTB(com_port + 1, 0x00);
  OUTB(com_port + 3, 0x80);
  OUTB(com_port + 0, baud_rate);
  OUTB(com_port + 1, 0x00);
  OUTB(com_port + 3, bit_type);
  OUTB(com_port + 2, IBMPC_SERIAL_FIFO_8);
  OUTB(com_port + 4, 0x08);

  return (ERROR_NONE);
}
