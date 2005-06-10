/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.c
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Fri Jun 10 15:45:31 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

t_cons                  cons =
  {
    .line = 0,
    .column = 0,
    .attr = CONS_ATTR,
    .vga = (char*)CONS_ADDR
  };

/*
 * this function just clears the console.
 */

void                    cons_clear(void)
{
  t_uint16              i;

  for (i = 0; i < CONS_SIZE; i++)
    {
      cons.vga[i] = 0;
      cons.vga[i + 1] = cons.attr;
    }

  cons.line = 0;
  cons.column = 0;
}

/*
 * this function is called by the printf function to change
 * the current console attributes.
 */

void			cons_attr(t_uint8		attr)
{
  cons.attr = attr;
}

/*
 * this function is called by the printf function to print a
 * character.
 */

void                    cons_print_char(char            c)
{
  t_uint16              pos = cons.line * CONS_COLUMNS * CONS_BPC +
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

void		kaneton(t_init*				init)
{
  printf("kaneton loaded successfully\n");

  printf("&init: 0x%x\n", init);

  while (1);
}
