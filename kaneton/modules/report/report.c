/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/modules/report/report.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [thu nov 25 11:19:24 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this module provides a functionality for reporting and compiling errors
 * in order to display a hiearchical report should an error occur.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_printf_char_fn		printf_char;
extern t_printf_attr_fn		printf_attr;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the module structure.
 */

m_module_report		_module_report;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the report.
 */

void			module_report_dump(void)
{
  t_sint32		i;
  t_sint32		j;

  if (_module_report.offset == 0)
    return;

  printf("[!] report:\n");

  for (i = _module_report.offset - 1, j = i; i >= 0; i--)
    {
      if (_module_report.buffer[i] == '\0' && (j - i) > 0)
	{
	  printf("[!]   %s\n", _module_report.buffer + i + 1);

	  j = i;
	}
    }

  if ((j - i) > 0)
    printf("[!]   %s\n", _module_report.buffer + i + 1);
}

/*
 * this function is called by report_record(), through printf(), and records
 * a single character in the static report buffer.
 */

int			module_report_character(char		c)
{
  _module_report.buffer[_module_report.offset++] = c;

  return (1);
}

/*
 * this function is called by printf to modify the printing attributes.
 *
 * in the report module's context, this functionality is simply ignored.
 */

void			module_report_attribute(t_uint8		attribute)
{
}

/*
 * this function records an error message.
 *
 * steps:
 *
 * 1) save the printf()'s function pointers.
 * 2) initialize printf() with special function pointers.
 * 3) forward the call to vprintf() so that the format and arguments
 *    can be treated.
 * 4) mark the end of the recorded message.
 * 5) restore the printf()'s function pointers.
 */

void			module_report_record(char*		fmt,
					     ...)
{
  va_list		args;

  /*
   * 1)
   */

  _module_report.printf.character = printf_char;
  _module_report.printf.attribute = printf_attr;

  /*
   * 2)
   */

  printf_init(module_report_character, module_report_attribute);

  /*
   * 3)
   */

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);

  /*
   * 4)
   */

  _module_report.buffer[_module_report.offset++] = '\0';

  /*
   * 5)
   */

  printf_char = _module_report.printf.character;
  printf_attr = _module_report.printf.attribute;
}

/*
 * this function initializes the report module.
 */

t_error			module_report_initialize(void)
{
  module_call(console, console_message,
	      '+', "report module loaded\n");

  _module_report.offset = 0;

  MODULE_LEAVE();
}

/*
 * this function cleans everything.
 */

t_error			module_report_clean(void)
{
  MODULE_LEAVE();
}
