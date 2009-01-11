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

#include <libc/libc.h>
#include <kaneton.h>

#include <platform/platform.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * compute packet checksum.
 */

static t_uint32		chk_sum(void				*data,
				unsigned int			size)
{
  t_uint32		crc = 0;
  t_uint8		*cdata;

  cdata = data;
  while (size--)
    crc ^= *cdata++;

  return (crc);
}

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
 * this function sends a chunk of data.
 */

int			serial_send(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32			size)
{
  ibmpc_serial_write(com_port, data, size);
  return 0;
}

/*
 * this function receives a chunk of data.
 */

int			serial_recv(t_uint32			com_port,
				    t_serial_data		*rdata)
{
	ibmpc_serial_read(com_port, (t_uint8 *)rdata, sizeof(*rdata));
	return (0);
}

/*
 * this function is used to output data on serial port.
 */

int			serial_put(char				c)
{
  static int n = 0;
  static char buffer[513];

  if (c != '\n' && n < 511 && c != -1)
    buffer[n++] = c;
  else
    {
      if (c != -1)
      {
        if (c == '\n')
          buffer[n++] = '\r';
	buffer[n++] = c;
      }
#ifndef DEBUG_SERIAL // XXX
      //serial_send(SERIAL_PRIMARY, (t_uint8*)"printf", 6);
#endif
      serial_send(SERIAL_PRIMARY, (t_uint8*)buffer, n);
      n = 0;
    }
  return (1);
}

/*
 * this function initializes the serial port.
 */

t_error			serial_init(t_uint32			com_port,
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
