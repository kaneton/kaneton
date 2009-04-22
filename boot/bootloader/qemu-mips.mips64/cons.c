/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-virtual/cons.c
 *
 * created       julien quintard   [sat may 28 18:23:13 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:16:28 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "cons.h"

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just clears the console.
 */

void			bootloader_cons_clear(void)
{
}

/*
 * this function scrolls the screen.
 */

void			bootloader_cons_scroll(t_uint16		lines)
{
}

/*
 * this function is called by the printf function to change the
 * current console attributes with the sequence %#.
 */

void			bootloader_cons_attr(t_sint8		attr)
{
}

/*
 * this function is called by the printf function to print a
 * character.
 */

int			bootloader_cons_print_char(char		c)
{
  if (c == 13 || c == 10)
    {
      write_char(10);
      write_char(13);
    }
  else
    write_char(c);
  
  return 1;
}

/*
 * this function just prints a string.
 */

void			bootloader_cons_print_string(char*	string)
{
  t_uint32		i;

  for (i = 0; string[i]; i++)
    bootloader_cons_print_char(string[i]);
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			bootloader_cons_msg(char		indicator,
					    char*		fmt,
					    ...)
{
  va_list		args;

  bootloader_cons_print_char('[');

  bootloader_cons_print_char(indicator);

  bootloader_cons_print_string("] ");

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);
}

/*
 * this function just initializes the bootloader console.
 *
 * this function also initializes the printf function to work with
 * the console.
 */

int			bootloader_cons_init(void)
{
  printf_init(bootloader_cons_print_char, bootloader_cons_attr);
  return 0;
}
