/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-octane.mips64/cons.c
 *
 * created       enguerrand raymond   [sun oct 22 21:34:14 2006]
 * updated       enguerrand raymond   [sun oct 22 22:03:52 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function is called by the printf function to print a
 * character.
 */

int	bootloader_cons_print_char(char	c)
{
  return (write(c));
}

/*
 * this function is called by the printf function to change the
 * current console attributes with the sequence %#.
 */

void	bootloader_cons_attr(u_int8_t	attr)
{
}

/*
 * this function just initialises the bootloader console.
 *
 * this function also initialises the printf function to work with
 * the console.
 */

int	bootloader_cons_init(void)
{
  printf_init(bootloader_cons_print_char, bootloader_cons_attr);
  return (0);
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
  //  t_uint8		attr = cons.attr;
  va_list		args;

  //  cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  bootloader_cons_print_char('[');

  /*switch (indicator)
    {
    case '+':
      cons.attr = CONS_FRONT(CONS_GREEN) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    case '#':
      cons.attr = CONS_FRONT(CONS_MAGENTA) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    case '!':
      cons.attr = CONS_FRONT(CONS_RED) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
      }*/

  bootloader_cons_print_char(indicator);

  //cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  //bootloader_cons_print_string("] ");
  printf("] ");
  //cons.attr = attr;

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);
}
