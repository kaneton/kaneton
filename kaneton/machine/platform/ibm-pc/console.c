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

t_ibmpc_console		ibmpc_console;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just clears the console.
 */

void			ibmpc_console_clear(void)
{
  t_uint16		i;

  for (i = 0; i < IBMPC_CONSOLE_SIZE; i++)
    {
      ibmpc_console.vga[i + 0] = 0;
      ibmpc_console.vga[i + 1] = ibmpc_console.attribute;
    }

  ibmpc_console.line = 0;
  ibmpc_console.column = 0;
}

/*
 * this function scrolls the screen.
 */

void			ibmpc_console_scroll(t_uint16		lines)
{
  t_uint16		src;
  t_uint16		dst;

  for (src = lines * IBMPC_CONSOLE_COLUMNS * IBMPC_CONSOLE_BPC, dst = 0;
       src < IBMPC_CONSOLE_SIZE;
       src++, dst++)
    ibmpc_console.vga[dst] = ibmpc_console.vga[src];

  for (src = (IBMPC_CONSOLE_LINES - lines) *
	 IBMPC_CONSOLE_COLUMNS * IBMPC_CONSOLE_BPC;
       src < IBMPC_CONSOLE_SIZE;
       src += 2)
    {
      ibmpc_console.vga[src + 0] = 0;
      ibmpc_console.vga[src + 1] = ibmpc_console.attribute;
    }

  ibmpc_console.line -= lines;
  ibmpc_console.column = 0;
}

/*
 * this function is called by the printf function to change the
 * current console attributes with the sequence %#.
 */

void			ibmpc_console_attribute(t_uint8		attribute)
{
  ibmpc_console.attribute = attribute;
}

/*
 * this function is called by the printf function to print a
 * character.
 */

int			ibmpc_console_print_char(char		c)
{
  t_uint16		pos;

  if (ibmpc_console.line >= IBMPC_CONSOLE_LINES)
    ibmpc_console_scroll(1);
  if (c == '\n')
    {
      ibmpc_console.line++;
      ibmpc_console.column = 0;

      return (1);
    }

  if (c == '\r')
    {
      ibmpc_console.column = 0;

      return (1);
    }

  if (c == '\t')
    {
      ibmpc_console.column += 8;
      if (ibmpc_console.column & 0x7)
	ibmpc_console.column = ibmpc_console.column & ~0x7;

      return (1);
    }

  if (ibmpc_console.column >= IBMPC_CONSOLE_COLUMNS)
    {
      ibmpc_console.column = 0;
      ++ibmpc_console.line;
      if (ibmpc_console.line >= IBMPC_CONSOLE_LINES)
	ibmpc_console_scroll(1);
    }

  pos = ibmpc_console.line * IBMPC_CONSOLE_COLUMNS * IBMPC_CONSOLE_BPC +
    ibmpc_console.column * IBMPC_CONSOLE_BPC;

  ibmpc_console.vga[pos] = c;
  ibmpc_console.vga[pos + 1] = ibmpc_console.attribute;

  ibmpc_console.column++;

  return (1);
}

/*
 * this function just prints a string.
 */

void			ibmpc_console_print_string(char*	string)
{
  t_uint32		i;

  for (i = 0; string[i]; i++)
    ibmpc_console_print_char(string[i]);
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			ibmpc_console_message(char		indicator,
					      char*		fmt,
					      va_list		args)
{
  t_uint8		attribute = ibmpc_console.attribute;

  ibmpc_console.attribute = IBMPC_CONSOLE_FRONT(IBMPC_CONSOLE_BLUE) |
    IBMPC_CONSOLE_BACK(IBMPC_CONSOLE_BLACK) | IBMPC_CONSOLE_INT;
  printf("[");

  switch (indicator)
    {
    case '+':
      ibmpc_console.attribute = IBMPC_CONSOLE_FRONT(IBMPC_CONSOLE_GREEN) |
	IBMPC_CONSOLE_BACK(IBMPC_CONSOLE_BLACK) | IBMPC_CONSOLE_INT;
      break;
    case '#':
      ibmpc_console.attribute = IBMPC_CONSOLE_FRONT(IBMPC_CONSOLE_MAGENTA) |
	IBMPC_CONSOLE_BACK(IBMPC_CONSOLE_BLACK) | IBMPC_CONSOLE_INT;
      break;
    case '!':
      ibmpc_console.attribute = IBMPC_CONSOLE_FRONT(IBMPC_CONSOLE_RED) |
	IBMPC_CONSOLE_BACK(IBMPC_CONSOLE_BLACK) | IBMPC_CONSOLE_INT;
      break;
    }

  printf("%c", indicator);

  ibmpc_console.attribute = IBMPC_CONSOLE_FRONT(IBMPC_CONSOLE_BLUE) |
    IBMPC_CONSOLE_BACK(IBMPC_CONSOLE_BLACK) | IBMPC_CONSOLE_INT;
  printf("] ");

  ibmpc_console.attribute = attribute;

  vprintf(fmt, args);
}

/*
 * this function just initializes the bootloader console.
 *
 * this function also initializes the printf function to work with the
 * console.
 */

t_error			ibmpc_console_initialize(void)
{
  ibmpc_console.attribute = IBMPC_CONSOLE_FRONT(IBMPC_CONSOLE_WHITE) |
    IBMPC_CONSOLE_BACK(IBMPC_CONSOLE_BLACK) | IBMPC_CONSOLE_INT;
  ibmpc_console.vga = (char*)IBMPC_CONSOLE_ADDR;

  ibmpc_console_clear();

  printf_init(ibmpc_console_print_char, ibmpc_console_attribute);

  return (ERROR_NONE);
}

/*
 * this function just reinitializes the bootloader console.
 *
 * there is nothing special to do.
 */

t_error			ibmpc_console_clean(void)
{
  return (ERROR_NONE);
}
