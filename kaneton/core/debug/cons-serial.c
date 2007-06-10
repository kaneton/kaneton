/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/debug/cons-serial.c
 *
 * created       matthieu bucchianeri   [tue jul 11 00:32:13 2006]
 * updated       matthieu bucchianeri   [tue feb  6 22:24:29 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * serial port console.
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
 * print a single char.
 */

int	cons_putc(char c)
{
  serial_write(SERIAL_COM1, (t_uint8*)&c, 1);

  return 1;
}


/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			cons_msg(char				indicator,
				 char*				fmt,
				 ...)
{
  va_list		args;

  printf("[%c] ", indicator);

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);
}



/*
 * this function just initialises the bootloader console.
 *
 * this function also initialises the printf function to work with the
 * console.
 */

t_error			cons_init(void)
{
  serial_init(SERIAL_COM1, SERIAL_BR57600, SERIAL_8N1, SERIAL_FIFO_8);

  printf_init(cons_putc, 0);

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the bootloader console.
 *
 * there is nothing special to do.
 */

t_error			cons_clean(void)
{
  return (ERROR_NONE);
}
