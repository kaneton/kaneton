/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/modules/console/console.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [thu apr  7 15:13:01 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the in-kernel console utility.
 *
 * this module is partially portable since it relies on the platform's console
 * functionality.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the console module manager.
 */

mm_console		_module_console;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function is called by format() for displaying a character.
 */

void			module_console_character(char		c)
{
  if (_module_console.character == NULL)
    return;

  _module_console.character(c);
}

/*
 * this function is called by format() for setting the attribute.
 */

void			module_console_attribute(t_uint8	attribute)
{
  if (_module_console.attribute == NULL)
    return;

  _module_console.attribute(attribute);
}

/*
 * this function displays an advanced message.
 *
 * steps:
 *
 * 1) display the opening bracket.
 * 2) display the indicator in a specific color.
 * 3) display the closing bracket.
 * 4) display the space.
 * 5) call the format() function to treat the arguments.
 */

void			module_console_message(char		indicator,
					       char*		fmt,
					       ...)
{
  va_list		args;

  /*
   * 1)
   */

  module_console_attribute(MODULE_CONSOLE_FRONT(MODULE_CONSOLE_BLUE) |
			   MODULE_CONSOLE_BACK(MODULE_CONSOLE_BLACK) |
			   MODULE_CONSOLE_BRIGHT);

  module_console_character('[');

  /*
   * 2)
   */

  switch (indicator)
    {
    case '+':
      module_console_attribute(MODULE_CONSOLE_FRONT(MODULE_CONSOLE_GREEN) |
			       MODULE_CONSOLE_BACK(MODULE_CONSOLE_BLACK) |
			       MODULE_CONSOLE_BRIGHT);
      break;
    case '#':
      module_console_attribute(MODULE_CONSOLE_FRONT(MODULE_CONSOLE_MAGENTA) |
			       MODULE_CONSOLE_BACK(MODULE_CONSOLE_BLACK) |
			       MODULE_CONSOLE_BRIGHT);
      break;
    case '!':
      module_console_attribute(MODULE_CONSOLE_FRONT(MODULE_CONSOLE_RED) |
			       MODULE_CONSOLE_BACK(MODULE_CONSOLE_BLACK) |
			       MODULE_CONSOLE_BRIGHT);
      break;
    }

  module_console_character(indicator);

  /*
   * 3)
   */

  module_console_attribute(MODULE_CONSOLE_FRONT(MODULE_CONSOLE_BLUE) |
			   MODULE_CONSOLE_BACK(MODULE_CONSOLE_BLACK) |
			   MODULE_CONSOLE_BRIGHT);

  module_console_character(']');

  /*
   * 4)
   */

  module_console_attribute(MODULE_CONSOLE_FRONT(MODULE_CONSOLE_WHITE) |
			   MODULE_CONSOLE_BACK(MODULE_CONSOLE_BLACK) |
			   MODULE_CONSOLE_BRIGHT);

  module_console_character(' ');

  /*
   * 5)
   */

  va_start(args, fmt);

  format(module_console_character, module_console_attribute,
	 fmt, args);

  va_end(args);
}

/*
 * this function prints a formatted text to the screen.
 */

void			module_console_print(char*		fmt,
					     ...)
{
  va_list		args;

  va_start(args, fmt);

  format(module_console_character, module_console_attribute,
	 fmt, args);

  va_end(args);
}

/*
 * this function sets the console configuration pointers.
 */

t_status		module_console_set(mf_console_character	character,
					   mf_console_attribute	attribute)
{
  _module_console.character = character;
  _module_console.attribute = attribute;

  MODULE_LEAVE();
}

/*
 * this function returns the console configuration pointers.
 */

t_status		module_console_get(mf_console_character* character,
					   mf_console_attribute* attribute)
{
  *character = _module_console.character;
  *attribute = _module_console.attribute;

  MODULE_LEAVE();
}

/*
 * this function loads the module.
 *
 * steps:
 *
 * 1) initialize the console structure.
 * 2) initialize the platform console.
 * 3) set the initialize configuration pointers to forward the calls
 *    to the platform.
 * 4) initialize the console attribute.
 * 5) display a message.
 */

t_status		module_console_load(void)
{
  /*
   * 1)
   */

  memset(&_module_console, 0x0, sizeof (mm_console));

  /*
   * 2)
   */

  if (platform_console_initialize() != STATUS_OK)
    MODULE_ESCAPE("unable to initialize the platform's console");

  /*
   * 3)
   */

  module_console_set(platform_console_character,
		     platform_console_attribute);

  /*
   * 4)
   */

  module_console_attribute(MODULE_CONSOLE_FRONT(MODULE_CONSOLE_WHITE) |
			   MODULE_CONSOLE_BACK(MODULE_CONSOLE_BLACK) |
			   MODULE_CONSOLE_BRIGHT);

  /*
   * 5)
   */

  module_call(console, message,
	      '+', "loading the 'console' module\n");

  MODULE_LEAVE();
}

/*
 * this function unloads the module.
 *
 * steps:
 *
 * 1) display a message.
 * 2) clean the underlying platform console.
 */

t_status		module_console_unload(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "unloading the 'console' module\n");

  /*
   * 2)
   */

  if (platform_console_clean() != STATUS_OK)
    MODULE_ESCAPE("unable to clean the platform's console");

  MODULE_LEAVE();
}
