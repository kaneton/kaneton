/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/debug/cons-text.c
 *
 * created       quintard julien   [sat may 28 18:23:13 2005]
 * updated       matthieu bucchianeri   [tue feb  6 22:22:47 2007]
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
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the console variable.
 */

m_platform_console	_platform_console;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just clears the console.
 */

void			platform_console_clear(void)
{
  t_uint16		i;

  for (i = 0; i < PLATFORM_CONSOLE_SIZE; i++)
    {
      _platform_console.vga[i + 0] = 0;
      _platform_console.vga[i + 1] = _platform_console.attribute;
    }

  _platform_console.line = 0;
  _platform_console.column = 0;
}

/*
 * this function scrolls the screen.
 */

void			platform_console_scroll(t_uint16	lines)
{
  t_uint16		src;
  t_uint16		dst;

  for (src = lines * PLATFORM_CONSOLE_COLUMNS * PLATFORM_CONSOLE_BPC, dst = 0;
       src < PLATFORM_CONSOLE_SIZE;
       src++, dst++)
    _platform_console.vga[dst] = _platform_console.vga[src];

  for (src = (PLATFORM_CONSOLE_LINES - lines) *
	 PLATFORM_CONSOLE_COLUMNS * PLATFORM_CONSOLE_BPC;
       src < PLATFORM_CONSOLE_SIZE;
       src += 2)
    {
      _platform_console.vga[src + 0] = 0;
      _platform_console.vga[src + 1] = _platform_console.attribute;
    }

  _platform_console.line -= lines;
  _platform_console.column = 0;
}

/*
 * this function is called by the printf function to change the
 * current console attributes with the sequence %#.
 */

void			platform_console_attribute(t_uint8	attribute)
{
  _platform_console.attribute = attribute;
}

/*
 * this function is called by the printf function to print a
 * character.
 */

int			platform_console_print_char(char	c)
{
#ifdef MODULE_test
  module_call(test, test_write, c);
#else
  t_uint16		pos;

  if (_platform_console.line >= PLATFORM_CONSOLE_LINES)
    platform_console_scroll(1);
  if (c == '\n')
    {
      _platform_console.line++;
      _platform_console.column = 0;

      return (1);
    }

  if (c == '\r')
    {
      _platform_console.column = 0;

      return (1);
    }

  if (c == '\t')
    {
      _platform_console.column += 8;
      if (_platform_console.column & 0x7)
	_platform_console.column = _platform_console.column & ~0x7;

      return (1);
    }

  if (_platform_console.column >= PLATFORM_CONSOLE_COLUMNS)
    {
      _platform_console.column = 0;
      ++_platform_console.line;
      if (_platform_console.line >= PLATFORM_CONSOLE_LINES)
	platform_console_scroll(1);
    }

  pos = _platform_console.line *
    PLATFORM_CONSOLE_COLUMNS * PLATFORM_CONSOLE_BPC +
    _platform_console.column * PLATFORM_CONSOLE_BPC;

  _platform_console.vga[pos] = c;
  _platform_console.vga[pos + 1] = _platform_console.attribute;

  _platform_console.column++;
#endif

  return (1);
}

/*
 * this function just prints a string.
 */

void			platform_console_print_string(char*	string)
{
  t_uint32		i;

  for (i = 0; string[i]; i++)
    platform_console_print_char(string[i]);
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			platform_console_message(char		indicator,
						 char*		fmt,
						 va_list	args)
{
  t_uint8		attribute = _platform_console.attribute;

  _platform_console.attribute =
    PLATFORM_CONSOLE_FRONT(PLATFORM_CONSOLE_BLUE) |
    PLATFORM_CONSOLE_BACK(PLATFORM_CONSOLE_BLACK) |
    PLATFORM_CONSOLE_INT;

  printf("[");

  switch (indicator)
    {
    case '+':
      _platform_console.attribute =
	PLATFORM_CONSOLE_FRONT(PLATFORM_CONSOLE_GREEN) |
	PLATFORM_CONSOLE_BACK(PLATFORM_CONSOLE_BLACK) |
	PLATFORM_CONSOLE_INT;
      break;
    case '#':
      _platform_console.attribute =
	PLATFORM_CONSOLE_FRONT(PLATFORM_CONSOLE_MAGENTA) |
	PLATFORM_CONSOLE_BACK(PLATFORM_CONSOLE_BLACK) |
	PLATFORM_CONSOLE_INT;
      break;
    case '!':
      _platform_console.attribute =
	PLATFORM_CONSOLE_FRONT(PLATFORM_CONSOLE_RED) |
	PLATFORM_CONSOLE_BACK(PLATFORM_CONSOLE_BLACK) |
	PLATFORM_CONSOLE_INT;
      break;
    }

  printf("%c", indicator);

  _platform_console.attribute =
    PLATFORM_CONSOLE_FRONT(PLATFORM_CONSOLE_BLUE) |
    PLATFORM_CONSOLE_BACK(PLATFORM_CONSOLE_BLACK) |
    PLATFORM_CONSOLE_INT;

  printf("] ");

  _platform_console.attribute = attribute;

  vprintf(fmt, args);
}

/*
 * this function just initializes the bootloader console.
 *
 * this function also initializes the printf function to work with the
 * console.
 */

t_error			platform_console_initialize(void)
{
  _platform_console.attribute =
    PLATFORM_CONSOLE_FRONT(PLATFORM_CONSOLE_WHITE) |
    PLATFORM_CONSOLE_BACK(PLATFORM_CONSOLE_BLACK) |
    PLATFORM_CONSOLE_INT;

  _platform_console.vga = (char*)PLATFORM_CONSOLE_ADDR;

  platform_console_clear();

  printf_init(platform_console_print_char, platform_console_attribute);

  MACHINE_LEAVE();
}

/*
 * this function just reinitializes the bootloader console.
 *
 * there is nothing special to do.
 */

t_error			platform_console_clean(void)
{
  MACHINE_LEAVE();
}
