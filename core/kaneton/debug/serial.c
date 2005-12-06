/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * serial.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sat May 28 18:23:13 2005   mycure
 * last update   Fri Oct 21 19:56:38 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */


t_uint32		chk_sum(void				*data,
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

void			serial_read(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32 			size)
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

void			serial_write(t_uint32			com_port,
				     t_uint8*			data,
				     t_uint32			size)
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

int			serial_send(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32			size)
{
  t_serial_data		sdata;
  t_uint32		crc;
  t_uint8		status[7];
  
  sdata.crc = chk_sum(data, size);
  sdata.size = size;	
  sdata.data = data;	
  sdata.magic = 0xF4859632;
  
  serial_write(com_port, (t_uint8 *) &sdata, sizeof(sdata));
  serial_write(com_port, sdata.data, sdata.size);
  serial_read(com_port, status, 6);
  status[6] = '\0';
  if (!strcmp(status, "crc-ok"))
    return (0);
  else
    return (-1);
}


int			serial_recv(t_uint32			com_port, 
				    t_serial_data		*rdata)
{
  t_uint32		crc;
  int			i;
  
  serial_read(com_port, (t_uint8 *)rdata, sizeof(*rdata));
  if (rdata->magic == 0xF4859632)
    rdata->data = malloc(rdata->size * sizeof(t_uint8));
  else
    {
      serial_write(com_port, "badcrc", 6);
      return (-1);
    }
  serial_read(com_port, rdata->data, rdata->size);
  if (rdata->crc == chk_sum(rdata->data, rdata->size))
    {
      serial_write(com_port, "crc-ok", 6);
      return (0);
    }
  else
    {
      serial_write(com_port, "badcrc", 6);
      free (rdata->data);
      return (-1);
    }
}

/*
 * this function just initialises the serial driver.
 */

int	serial_put(char c)
{
	static int n = 0;
	static char buffer[512];

	if (c != '\n' && n < 512 && c != -1)
	    buffer[n++] = c;
	else
	{
	 if (c != -1)
	  buffer[n++] = c;
	  buffer[n] = 0;
          serial_send(SERIAL_COM1, "printf", 6);
	  serial_send(SERIAL_COM1, buffer, n);
	  n = 0;
	}
}

void			serial_init(t_uint32			com_port, 
				    t_uint8			baud_rate, 
				    t_uint8			bit_type, 
				    t_uint8			fifo_type)
{
  /*
   * 1) Turn off interupt on choosen port 
   * 2) Set dlab on
   * 3) Set baud rate for choosen port
   * 4) Set connection type
   * 5) Set fifo type
   * 6) DTR, RTS, OUT2 : on
   */
 
  /* 1) */
  OUTB(com_port + 1, 0x00);
  /* 2) */
  OUTB(com_port + 3, 0x80);
  /* 3) */
  OUTB(com_port + 0, baud_rate);
  OUTB(com_port + 1, 0x00);
  /* 4) */
  OUTB(com_port + 3, bit_type);
  /* 5) */
  OUTB(com_port + 2, fifo_type);
  /* 6) */
  OUTB(com_port + 4, 0x08); 
}
