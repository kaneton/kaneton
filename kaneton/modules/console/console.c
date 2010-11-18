/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/kaneton/modules/console/console.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [sat nov 13 00:46:20 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the in-kernel console utility.
 *
 * this module is not portable and relies on the ibm-pc platform.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just clears the console.
 */

void			console_clear(void)
{
  ibmpc_console_clear();
}

/*
 * this function scrolls the screen.
 */

void			console_scroll(t_uint16			lines)
{
  ibmpc_console_scroll(lines);
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			console_message(char			indicator,
					char*			fmt,
					...)
{
  va_list		args;

  va_start(args, fmt);

  ibmpc_console_message(indicator, fmt, args);

  va_end(args);
}

/*
 * this function just initializes the console.
 */

t_error			console_initialize(void)
{
  return (ibmpc_console_initialize());
}

/*
 * this function cleans everything.
 */

t_error			console_clean(void)
{
  return (ibmpc_console_clean());
}
