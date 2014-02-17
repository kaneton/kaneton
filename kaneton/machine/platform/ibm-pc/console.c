/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...aneton/machine/platform/ibm-pc/console.c
 *
 * created       julien quintard   [sat may 28 18:23:13 2005]
 * updated       julien quintard   [thu apr  7 15:13:32 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements basic console functionalities. these functionalities
 * are mainly use during the kernel boot though a dedicated console service
 * will be launched by the 'system' initial service.
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
 * the console manager.
 */

pm_console		_platform_console;

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
 * this function scrolls the screen by copying the given number of lines
 * from the bottom to the top.
 *
 * finally the function reinitializes the attributes to reflect the
 * scrolling.
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
 * this function changes the current console attributes.
 */

void			platform_console_attribute(t_uint8	attribute)
{
  _platform_console.attribute = attribute;
}

/*
 * this function prints a single character.
 *
 * first, should the number of lines exceed the maximum capacity, the screen
 * is scrolled for a single line.
 *
 * the character is recorded at the cursor's current location while the
 * following byte is filled with the current console's attributes.
 *
 * steps:
 *
 * 1) scroll the screen if necessary.
 * 2) handle the '\n' character.
 * 3) handle the '\r' character.
 * 4) handle the '\t' character.
 * 5) insert the character in the console's mapped buffer.
 */

void			platform_console_character(char			c)
{
  t_uint16		pos;

  /*
   * 1)
   */

  if (_platform_console.line >= PLATFORM_CONSOLE_LINES)
    {
      platform_console_scroll(1);
    }

  /*
   * 2)
   */

  if (c == '\n')
    {
      _platform_console.line++;
      _platform_console.column = 0;

      return;
    }

  /*
   * 3)
   */

  if (c == '\r')
    {
      _platform_console.column = 0;

      return;
    }

  /*
   * 4)
   */

  if (c == '\t')
    {
      _platform_console.column += 8;

      if (_platform_console.column & 0x7)
	_platform_console.column = _platform_console.column & ~0x7;

      return;
    }

  /*
   * 5)
   */

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
 * this function displays a string, character by character.
 */

void			platform_console_string(char*		string)
{
  t_uint32		i;

  for (i = 0; string[i] != '\0'; i++)
    platform_console_character(string[i]);
}

/*
 * this function just initializes the console.
 *
 * steps:
 *
 * 1) initialize the manager's structure memory.
 * 2) set the default attributes.
 * 3) points the internal buffer to the device's mapped memory.
 * 4) to start with, clear the console.
 */

t_status		platform_console_initialize(void)
{
  /*
   * 1)
   */

  memset(&_platform_console, 0x0, sizeof (pm_console));

  /*
   * 2)
   */

  _platform_console.attribute =
    PLATFORM_CONSOLE_FRONT(PLATFORM_CONSOLE_WHITE) |
    PLATFORM_CONSOLE_BACK(PLATFORM_CONSOLE_BLACK) |
    PLATFORM_CONSOLE_BRIGHT;

  /*
   * 3)
   */

  _platform_console.vga = (char*)PLATFORM_CONSOLE_ADDRESS;

  /*
   * 4)
   */

  platform_console_clear();

  MACHINE_LEAVE();
}

/*
 * this function just reinitializes the bootloader console.
 *
 * there is nothing special to do.
 */

t_status		platform_console_clean(void)
{
  MACHINE_LEAVE();
}
