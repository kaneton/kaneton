/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/bootloader/arch/ia32-virtual/cons.c
 *
 * created       julien quintard   [sat may 28 18:23:13 2005]
 * updated       matthieu bucchianeri   [tue jan 24 11:22:46 2006]
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * the student has to fill in this file just to be able to print some
 * debug messages.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * the console variable.
 */

t_cons			cons;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k1 */

/*
 * this function just clears the console.
 */

void			bootloader_cons_clear(void)
{
  t_uint16		i;

  for (i = 0; i < CONS_SIZE; i++)
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

  for (src = lines * CONS_COLUMNS * CONS_BPC, dst = 0;
       src < CONS_SIZE;
       src++, dst++)
    cons.vga[dst] = cons.vga[src];

  for (src = (CONS_LINES - lines) * CONS_COLUMNS * CONS_BPC;
       src < CONS_SIZE;
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

void			bootloader_cons_attr(t_uint8		attr)
{
  cons.attr = attr;
}

/*
 * this function is called by the printf function to print a
 * character.
 */

int			bootloader_cons_print_char(char		c)
{
  t_uint16		pos = cons.line * CONS_COLUMNS * CONS_BPC +
    cons.column * CONS_BPC;

  if (c == '\n')
    {
      cons.line++;
      cons.column = 0;

      return (0);
    }

  if (pos >= CONS_SIZE)
    {
      bootloader_cons_scroll(1);

      pos = cons.line * CONS_COLUMNS * CONS_BPC + cons.column * CONS_BPC;
    }

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

  cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  bootloader_cons_print_char('[');

  switch (indicator)
    {
    case '+':
      cons.attr = CONS_FRONT(CONS_GREEN) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    case '#':
      cons.attr = CONS_FRONT(CONS_MAGENTA) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    case '!':
      cons.attr = CONS_FRONT(CONS_RED) | CONS_BACK(CONS_BLACK) | CONS_INT;
      break;
    }

  bootloader_cons_print_char(indicator);

  cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  bootloader_cons_print_string("] ");

  cons.attr = attr;

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);
}

/*
 * this function loads the current console state into the init
 * variable to pass it to the kernel.
 *
 * this step is just used to keep the console in the same state between
 * the bootloader and the kernel.
 */

void			bootloader_cons_load(void)
{
  memcpy(&init->machdep.cons, &cons, sizeof(t_cons));
}

/*
 * this function just initialises the bootloader console.
 *
 * this function also initialises the printf function to work with
 * the console.
 */

int			bootloader_cons_init(void)
{
  cons.line = 0;
  cons.column = 0;
  cons.attr = CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons.vga = (char*)CONS_ADDR;

  bootloader_cons_clear();

  printf_init(bootloader_cons_print_char, bootloader_cons_attr);

  return (0);
}

/*                                                                 [cut] /k1 */
