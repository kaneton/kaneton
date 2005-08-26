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

#define COM1 	0x3f8

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * XXX
 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function reads on the serial device.
 */

int 	serial_get_char(void)
{
	int c;
	
	while (1)
	{
		INB(COM1 + 5, c);
		if ((c & 0x01))
		{
		INB(COM1, c);
		return (c);
		}	
	}
}


int			serial_read(t_uint8*			data, int size, int* n)
{
 
	int i;
	int c;
	
	for (i = 0; i < size; i++)
	{
		*data = (char) serial_get_char();
		data++;
	}
	/*vider le buffer */
	*data = '\0';
	
	
	/*
   * XXX
   */
/*	*n = 0;
	
	while (size) 
	{
	INB(COM1 + 5, *data);   ou si y a encore des caractere ds le buffer
	  if (*data & 1)
	     {
		INB(COM1, *data);
		if (!*data)
		   return (*n);	
		data++;
		size--;
		*n++;
	     }
	}
  *data = '\0';
	*/
  return (0);
}

/*
 * this function writes on the serial device
 */

int			serial_write(t_uint8*			data,	int		size, int* n)
{
  /*
   * XXX
   */
  *n = 0;
	
  while(size--)
  {
  OUTB(COM1, *data++);
  *n++;
  }
  
  return (0);
}

/*
 * this function just initialises the serial driver.
 */

int			serial_init(void)
{
  /*
   * XXX
   */
	char buff[1024];
	
	printf("init rs-232\n");
	int n;	
	/* POLL MODE TEST */

	int i = 0;

	OUTB(COM1 + 1, 0x00); /*turn of interupts port 1 */
	OUTB(COM1 + 3, 0x80); /*set dlab on*/
	OUTB(COM1 + 0, 0x0C); /*set divisor = 12  == 9.3 kbps*/
	OUTB(COM1 + 1, 0x00); /* set divisor high byte */	
	OUTB(COM1 + 3, 0x03); /* 8 bits no paritry 1 stop bit == 8N1 */ 
	OUTB(COM1 + 2, 0x87); /* FIFO controle register  8*/
	OUTB(COM1 + 4, 0x08); /* on : DTR RTS OUT2*/

/*	serial_write("Test\n", 5, &n);*/
	while (1)
	{
	serial_read(buff, 10, &n);
	printf("%s\n", buff);
	}

	return (0);
}

/*
 * this function reinitialises the serial driver.
 */

int			serial_clean(void)
{
  /*
   * XXX
   */

  return (0);
}
