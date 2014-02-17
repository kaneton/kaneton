/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/lec_l/kaneton/kaneton/machine/platform/octane/cons.c
 *
 * created       laurent lec   [fri mar 28 17:13:10 2008]
 * updated       laurent lec   [fri mar 28 17:13:10 2008]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements a basic console management for kernel bootup.
 *
 * in the  future, the console control  will be passed  to a dedicated
 * server.
 *
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
 * this function just clears the console.
 */

void			cons_clear(void)
{
}

/*
 * this function scrolls the screen.
 */

void			cons_scroll(t_uint16			lines)
{
}

/*
 * this function is called by the printf function to change the
 * current console attributes with the sequence %#.
 */

void			cons_attr(t_uint8			attr)
{
}

/*
 * this function is called by the printf function to print a
 * character.
 */

int			cons_print_char(char			c)
{
  return (1);
}

/*
 * this function just prints a string.
 */

void			cons_print_string(char*			string)
{
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			module_call(console, console_message, char				indicator,
				 char*				fmt,
				 ...)
{
}

/*
 * this function just initializes the bootloader console.
 *
 * this function also initializes the printf function to work with the
 * console.
 */

t_status		cons_init(void)
{
  return (STATUS_OK);
}

/*
 * this function just reinitializes the bootloader console.
 *
 * there is nothing special to do.
 */

t_status		cons_clean(void)
{
  return (STATUS_OK);
}
