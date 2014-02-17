/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/modules/report/report.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [thu apr  7 15:13:06 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this module provides a functionality for reporting and compiling errors
 * in order to display a hiearchical report should an error occur.
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#if !defined(MODULE_console)
# error "the 'report' module depends upon the 'console' module"
#endif

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the module manager.
 */

mm_report		_module_report;

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

  module_call(console, message,
	      '!', "report:\n");

  for (i = _module_report.offset - 1, j = i; i >= 0; i--)
    {
      if (_module_report.buffer[i] == '\0' && (j - i) > 0)
	{
	  module_call(console, message,
		      '!', "  %s\n",
		      _module_report.buffer + i + 1);

	  j = i;
	}
    }

  if ((j - i) > 0)
    module_call(console, message,
		'!', "  %s\n",
		_module_report.buffer + i + 1);

  _module_report.offset = 0;
}

/*
 * this function is called by format() and records a single character
 * in the static report buffer.
 */

void			module_report_character(char		c)
{
  _module_report.buffer[_module_report.offset++] = c;
}

/*
 * this function is called by format() to specify the attribute.
 */

void			module_report_attribute(t_uint8		attribute)
{
}

/*
 * this function records an error message.
 *
 * steps:
 *
 * 1) call format() so that the the arguments can be treated.
 * 2) mark the end of the recorded message.
 */

void			module_report_record(char*		fmt,
					     ...)
{
  va_list			args;

  /*
   * 1)
   */

  va_start(args, fmt);

  format(module_report_character, module_report_attribute,
	 fmt, args);

  va_end(args);

  /*
   * 2)
   */

  _module_report.buffer[_module_report.offset++] = '\0';
}

/*
 * this function loads the report module.
 *
 * steps:
 *
 * 1) display a message.
 * 2) initialize the report structure.
 */

t_status		module_report_load(void)
{
  /*
   * 1)
   */

  module_call(console, message,
	      '+', "loading the 'report' module\n");

  /*
   * 2)
   */

  memset(&_module_report, 0x0, sizeof (mm_report));

  MODULE_LEAVE();
}

/*
 * this function unloads the module.
 */

t_status		module_report_unload(void)
{
  module_call(console, message,
	      '+', "unloading the 'report' module\n");

  MODULE_LEAVE();
}
