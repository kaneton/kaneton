/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * cons.c
 * 
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sat May 28 18:23:13 2005   mycure
 * last update   Fri Jun 10 15:39:16 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * the console variable.
 */

t_cons			cons;

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

void			cons_attr(t_uint8		attr)
{
  cons.attr = attr;
}

/*
 * this function is called by the printf function to print a
 * character.
 */

void			cons_print_char(char		c)
{
  t_uint16		pos = cons.line * CONS_COLUMNS * CONS_BPC +
    cons.column * CONS_BPC;

  if (c == '\n')
    {
      cons.line++;
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
}

/*
 * this function just prints a string.
 */

void			cons_print_string(char*	string)
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
 * '!' is used for printing error messages.
 */

void			cons_msg(char		indicator,
				 char*		fmt,
				 ...)
{
  t_uint8		attr = cons.attr;
  va_list		args;

  cons.attr = 0x9;
  cons_print_char('[');

  switch (indicator)
    {
    case '+':
      cons.attr = 0xa;
      break;
    case '#':
      cons.attr = 0x5;
      break;
    case '!':
      cons.attr = 0xc;
      break;
    }

  cons_print_char(indicator);

  cons.attr = 0x9;
  cons_print_string("] ");

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

void			cons_load(void)
{
  memcpy(&init->machdep.cons, &cons, sizeof(t_cons));
}

/*
 * this function just initializes the bootloader console.
 */

void			cons_init(void)
{
  cons.line = 0;
  cons.column = 0;
  cons.attr = CONS_ATTR;
  cons.vga = (char*)CONS_ADDR;

  cons_clear();

  cons_msg('+', "console manager initialized\n");
}
