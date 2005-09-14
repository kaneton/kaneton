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
 * last update   Tue Aug 23 23:08:19 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function reads on the serial device.
 */

t_uint32	chk_sum(void	*data, unsigned int size)
{
	t_uint32 crc = 0;
	t_uint8 *cdata;

	cdata = data;	
	while (size--)
	crc ^= *cdata++;

	return (crc);
}

int			serial_send(t_uint32 		    com_port,
				    t_uint8*			data,
				    t_uint32			size)
{
	t_data		sdata;
	t_uint32	crc;
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
	  {
		  printf("good send\n");
		  return (0);
	  }
	  else
	  {
	    printf("bad send\n");
	    printf("crc: 0x%d\n", sdata.crc);
	    return (-1);
	  }
	  return (-1);
}

int			serial_receiv(t_uint32 com_port, t_data *rdata)
{
	t_uint32	crc;
	int 		i;
	
       	serial_read(com_port, (t_uint8 *)rdata, sizeof(*rdata));
	if (rdata->magic == 0xF4859632)
	rdata->data = malloc(rdata->size * sizeof(t_uint8));
	else
	{
	   printf("bad_header\n");
	   serial_write(com_port, "badcrc", 6);
	   return (-1);
	}
	serial_read(com_port, rdata->data, rdata->size);
	  if (rdata->crc == chk_sum(rdata->data, rdata->size))
	     {
	      printf("Good receive\n");
	      serial_write(com_port, "crc-ok", 6);
	      return (0);
	     }
	  else
	     {
	      printf("Bad receiv\n");
	      serial_write(com_port, "badcrc", 6);
	      free (rdata->data);
	      return (-1);
	     }
	      
}

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
	  printf("\rReading %d more Byte", size);
	}
	printf("\n");
}

/*
 * this function writes on the serial device
 */

void			serial_write(t_uint32			com_port,
				     t_uint8*			data,
				     t_uint32			size)
{
  char 	ch;
	 
  while(size)
  {
  INB(com_port + 5, ch);
    if (ch & 0x20)
	 {
  	    OUTB(com_port, *data++);
	    size--;
	 }
    printf("\rSending %d Byte", size);
  }
  printf("\n");
}

/*
 * this function just initialises the serial driver.
 */

void			serial_init(	t_uint32 com_port, 
					t_uint8 baud_rate, 
				    	t_uint8 bit_type, 
					t_uint8 fifo_type)
{
  /*
   * 1) Turn off interupt on choosen port 
   * 2) Set dlab on
   * 3) Set baud rate for choosen port
   * 4) Set connection type
   * 5) Set fifo type
   * 6) DTR, RTS, OUT2 : on
   */
	OUTB(com_port + 1, 0x00); /* 1) */
	OUTB(com_port + 3, 0x80); /* 2) */
	OUTB(com_port + 0, baud_rate); /* 3) */
	OUTB(com_port + 1, 0x00);
	OUTB(com_port + 3, bit_type); /* 4) */
	OUTB(com_port + 2, fifo_type); /* 5) */
	OUTB(com_port + 4, 0x08); /* 6) */ 
}
