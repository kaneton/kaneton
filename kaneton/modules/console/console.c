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
 * updated       julien quintard   [fri dec  3 22:57:25 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the in-kernel console utility.
 *
 * this module is not portable and relies on platform console functionality.
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

void			module_console_clear(void)
{
  platform_console_clear();
}

/*
 * this function scrolls the screen.
 */

void			module_console_scroll(t_uint16		lines)
{
  platform_console_scroll(lines);
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			module_console_message(char		indicator,
					       char*		fmt,
					       ...)
{
  va_list		args;

  va_start(args, fmt);

  platform_console_message(indicator, fmt, args);

  va_end(args);
}

/*
 * this function just initializes the console.
 */

t_error			module_console_initialize(void)
{
  if (platform_console_initialize() != ERROR_OK)
    MODULE_ESCAPE("unable to initialize the platform's console");

  module_call(console, console_message,
	      '+', "console module loaded\n");

  MODULE_LEAVE();
}

/*
 * this function cleans everything.
 */

t_error			module_console_clean(void)
{
  if (platform_console_clean() != ERROR_OK)
    MODULE_ESCAPE("unable to clean the platform's console");

  MODULE_LEAVE();
}
