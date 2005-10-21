/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * cons.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sat May 28 18:23:13 2005   mycure
 * last update   Fri Oct 21 19:56:18 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

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

/*
 * this function just clears the console.
 */

void			cons_clear(void)
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

void			cons_scroll(t_uint16			lines)
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

void			cons_attr(t_uint8			attr)
{
  cons.attr = attr;
}

/*
 * this function is called by the printf function to print a
 * character.
 */

int			cons_print_char(char			c)
{
  t_uint16		pos = cons.line * CONS_COLUMNS * CONS_BPC +
    cons.column * CONS_BPC;

  if (c == '\n')
    {
      cons.line++;
      cons.column = 0;

      return ;
    }
  
  if (c == '\r')
    {
      cons.column = 0;

      return ;
    }
  
  if (pos >= CONS_SIZE)
    {
      cons_scroll(1);

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

void			cons_print_string(char*			string)
{
  t_uint32		i;

  for (i = 0; string[i]; i++)
    cons_print_char(string[i]);
}

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			cons_msg(char				indicator,
				 char*				fmt,
				 ...)
{
  t_uint8		attr = cons.attr;
  va_list		args;

  cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons_print_char('[');

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

  cons_print_char(indicator);

  cons.attr = CONS_FRONT(CONS_BLUE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons_print_string("] ");

  cons.attr = attr;

  va_start(args, fmt);

  vprintf(fmt, args);

  va_end(args);
}

/*
 * this function just initialises the bootloader console.
 *
 * this function also initialises the printf function to work with the
 * console.
 */

t_error			cons_init(void)
{
  memcpy(&cons, &init->machdep.cons, sizeof(t_cons));

  printf_init(cons_print_char, cons_attr);

  return (ERROR_NONE);
}

/*
 * this function just reinitialises the bootloader console.
 *
 * there is nothing special to do.
 */

t_error			cons_clean(void)
{
  return (ERROR_NONE);
}

/*
 * XXX
 *
 * [+] ce gestionnaire est provisoire car cela devrait etre un service.
 */
