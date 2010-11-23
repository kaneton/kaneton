/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.NEW/kaneton/modules/report/report.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [tue nov 23 14:56:45 2010]
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

m_report			_report;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the report.
 */

void			report_dump(void)
{
  t_sint32		i;
  t_sint32		j;

  if (_report.offset == 0)
    return;

  printf("[!] report:\n");

  for (i = _report.offset - 1, j = i; i >= 0; i--)
    {
      if (_report.buffer[i] == '\0' && (j - i) > 0)
	{
	  printf("[!]   %s\n", _report.buffer + i + 1);

	  j = i;
	}
    }

  if ((j - i) > 0)
    printf("[!]   %s\n", _report.buffer + i + 1);
}

/*
 * this function is called by report_record(), through printf(), and records
 * a single character in the static report buffer.
 */

int			report_character(char			c)
{
  _report.buffer[_report.offset++] = c;
}

/*
 * this function is called by printf to modify the printing attributes.
 *
 * in the report module's context, this functionality is simply ignored.
 */

void			report_attribute(t_uint8		attribute)
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

void			report_record(char*			fmt,
				      ...)
{
  va_list		args;

  /*
   * 1)
   */

  _report.printf.character = printf_char;
  _report.printf.attribute = printf_attr;

  /*
   * 2)
   */

  printf_init(report_character, report_attribute);

  /*
   * 3)
   */

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);

  /*
   * 4)
   */

  _report.buffer[_report.offset++] = '\0';

  /*
   * 5)
   */

  printf_char = _report.printf.character;
  printf_attr = _report.printf.attribute;
}

/*
 * this function initializes the report module.
 */

t_error			report_initialize(void)
{
  module_call(console, console_message, '+', "report module loaded\n");

  _report.offset = 0;

  return (ERROR_OK);
}

/*
 * this function cleans everything.
 */

t_error			report_clean(void)
{
  return (ERROR_OK);
}
