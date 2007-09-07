/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-virtual/cons.c
 *
 * created       julien quintard   [sat may 28 18:23:13 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:16:28 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the console variable.
 */

t_ibmpc_cons		cons;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just clears the console.
 */

void			bootloader_cons_clear(void)
{
  t_uint16		i;

  for (i = 0; i < IBMPC_CONS_SIZE; i++)
    {
      cons.vga[i + 0] = 0;
      cons.vga[i + 1] = cons.attr;
    }

  cons.line = 0;
  cons.column = 0;
}

/*
 * this function scrolls the screen.
 */

void			bootloader_cons_scroll(t_uint16		lines)
{
  t_uint16		src;
  t_uint16		dst;

  for (src = lines * IBMPC_CONS_COLUMNS * IBMPC_CONS_BPC, dst = 0;
       src < IBMPC_CONS_SIZE;
       src++, dst++)
    cons.vga[dst] = cons.vga[src];

  for (src = (IBMPC_CONS_LINES - lines) * IBMPC_CONS_COLUMNS * IBMPC_CONS_BPC;
       src < IBMPC_CONS_SIZE;
       src += 2)
    {
      cons.vga[src + 0] = 0;
      cons.vga[src + 1] = cons.attr;
    }

  cons.line -= lines;
  cons.column = 0;
}

/*
 * this function is called by the printf function to change the
 * current console attributes with the sequence %#.
 */

void			bootloader_cons_attr(t_sint8		attr)
{
  cons.attr = attr;
}

/*
 * this function is called by the printf function to print a
 * character.
 */

int			bootloader_cons_print_char(char		c)
{
  t_uint16		pos;

  if (cons.line >= IBMPC_CONS_LINES)
    bootloader_cons_scroll(1);
  if (c == '\n')
    {
      cons.line++;
      cons.column = 0;

      return (1);
    }

  if (c == '\r')
    {
      cons.column = 0;

      return (1);
    }

  if (cons.column >= IBMPC_CONS_COLUMNS)
    {
      cons.column = 0;
      ++cons.line;
      if (cons.line >= IBMPC_CONS_LINES)
	bootloader_cons_scroll(1);
    }

  pos = cons.line * IBMPC_CONS_COLUMNS * IBMPC_CONS_BPC +
    cons.column * IBMPC_CONS_BPC;

  cons.vga[pos] = c;
  cons.vga[pos + 1] = cons.attr;

  cons.column++;

  return (1);
}

/*
 * this function just prints a string.
 */

void			bootloader_cons_print_string(char*	string)
{
  t_uint32		i;

  for (i = 0; string[i]; i++)
    bootloader_cons_print_char(string[i]);
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			bootloader_cons_msg(char		indicator,
					    char*		fmt,
					    ...)
{
  t_uint8		attr = cons.attr;
  va_list		args;

  cons.attr = IBMPC_CONS_FRONT(IBMPC_CONS_BLUE) |
    IBMPC_CONS_BACK(IBMPC_CONS_BLACK) | IBMPC_CONS_INT;
  bootloader_cons_print_char('[');

  switch (indicator)
    {
    case '+':
      cons.attr = IBMPC_CONS_FRONT(IBMPC_CONS_GREEN) |
	IBMPC_CONS_BACK(IBMPC_CONS_BLACK) | IBMPC_CONS_INT;
      break;
    case '#':
      cons.attr = IBMPC_CONS_FRONT(IBMPC_CONS_MAGENTA) |
	IBMPC_CONS_BACK(IBMPC_CONS_BLACK) | IBMPC_CONS_INT;
      break;
    case '!':
      cons.attr = IBMPC_CONS_FRONT(IBMPC_CONS_RED) |
	IBMPC_CONS_BACK(IBMPC_CONS_BLACK) | IBMPC_CONS_INT;
      break;
    }

  bootloader_cons_print_char(indicator);

  cons.attr = IBMPC_CONS_FRONT(IBMPC_CONS_BLUE) |
    IBMPC_CONS_BACK(IBMPC_CONS_BLACK) | IBMPC_CONS_INT;
  bootloader_cons_print_string("] ");

  cons.attr = attr;

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);
}

/*
 * this function just initializes the bootloader console.
 *
 * this function also initializes the printf function to work with
 * the console.
 */

int			bootloader_cons_init(void)
{
  cons.line = 0;
  cons.column = 0;
  cons.attr = IBMPC_CONS_FRONT(IBMPC_CONS_WHITE) |
    IBMPC_CONS_BACK(IBMPC_CONS_BLACK) | IBMPC_CONS_INT;
  cons.vga = (char*)IBMPC_CONS_ADDR;

  bootloader_cons_clear();

  printf_init(bootloader_cons_print_char, bootloader_cons_attr);

  return (0);
}
