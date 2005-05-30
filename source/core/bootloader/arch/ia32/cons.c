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
 * last update   Sun May 29 15:56:47 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

t_cons			cons =
  {
    .line = 0,
    .column = 0,
    .attr = CONS_ATTR,
    .vga = (char*)CONS_ADDR
  };

/*
 * this function just clears the console.
 */

void			cons_clear(void)
{
  t_uint16		i;

  for (i = 0; i < CONS_SIZE; i++)
    {
      cons.vga[i] = 0;
      cons.vga[i + 1] = cons.attr;
    }

  cons.line = 0;
  cons.column = 0;
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
      cons.line = 0;
      cons.column = 0;

      cons_clear(); /* XXX */
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
    case '+':				/* ok */
      cons.attr = 0xa;
      break;
    case '#':				/* debug */
      cons.attr = 0x5;
      break;
    case '!':				/* error */
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
 * this function just initializes the bootloader console.
 */

void			cons_init(void)
{
  cons_clear();
}
