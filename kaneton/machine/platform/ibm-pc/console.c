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
 * this function is called to change the current console attributes.
 */

void			platform_console_attribute(t_uint8	attribute)
{
  _platform_console.attribute = attribute;
}

/*
 * this function is called to print a single character.
 */

void			platform_console_character(char			c)
{
  t_uint16		pos;

  if (_platform_console.line >= PLATFORM_CONSOLE_LINES)
    platform_console_scroll(1);
  if (c == '\n')
    {
      _platform_console.line++;
      _platform_console.column = 0;

      return;
    }

  if (c == '\r')
    {
      _platform_console.column = 0;

      return;
    }

  if (c == '\t')
    {
      _platform_console.column += 8;
      if (_platform_console.column & 0x7)
	_platform_console.column = _platform_console.column & ~0x7;

      return;
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
}

/*
 * this function just initializes the bootloader console.
 */

t_error			platform_console_initialize(void)
{
  _platform_console.attribute =
    PLATFORM_CONSOLE_FRONT(PLATFORM_CONSOLE_WHITE) |
    PLATFORM_CONSOLE_BACK(PLATFORM_CONSOLE_BLACK) |
    PLATFORM_CONSOLE_BRIGHT;

  _platform_console.vga = (char*)PLATFORM_CONSOLE_ADDRESS;

  platform_console_clear();

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
