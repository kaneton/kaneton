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

void			ibmpc_serial_read(t_uint32		com_port,
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
  t_serial_data		sdata;
  t_uint8		status[7];

  sdata.crc = chk_sum(data, size);
  sdata.size = size;
  sdata.data = data;
  sdata.magic = 0xF4859632;

  ibmpc_serial_write(com_port, (t_uint8 *) &sdata, sizeof(sdata));
  ibmpc_serial_write(com_port, sdata.data, sdata.size);
  ibmpc_serial_read(com_port, status, 6);
  status[6] = '\0';
  if (!strcmp((char*)status, "crc-ok"))
    return (0);
  else
    return (-1);
}

/*
 * this function receives a chunk of data.
 */

int			serial_recv(t_uint32			com_port,
				    t_serial_data		*rdata)
{
  ibmpc_serial_read(com_port, (t_uint8 *)rdata, sizeof(*rdata));
  if (rdata->magic == 0xF4859632)
    rdata->data = malloc(rdata->size * sizeof(t_uint8));
  else
    {
      ibmpc_serial_write(com_port, (t_uint8*)"badcrc", 6);
      return (-1);
    }
  ibmpc_serial_read(com_port, rdata->data, rdata->size);
  if (rdata->crc == chk_sum(rdata->data, rdata->size))
    {
      ibmpc_serial_write(com_port, (t_uint8*)"crc-ok", 6);
      return (0);
    }
  else
    {
      ibmpc_serial_write(com_port, (t_uint8*)"badcrc", 6);
      free (rdata->data);
      return (-1);
    }
}

/*
 * this function is used to output data on serial port.
 */

int			serial_put(char				c)
{
  static int n = 0;
  static char buffer[513];

  if (c != '\n' && n < 512 && c != -1)
    buffer[n++] = c;
  else
    {
      if (c != -1)
	buffer[n++] = c;
      buffer[n] = 0;
#ifndef DEBUG_SERIAL // XXX
      serial_send(SERIAL_PRIMARY, (t_uint8*)"printf", 6);
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
