/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/drivers/cons-simple/cons-simple.c
 *
 * created       matthieu bucchianeri   [sat jun  9 18:36:19 2007]
 * updated       matthieu bucchianeri   [mon sep 10 00:14:07 2007]
 */

#include "teton.h"

/*
 * console instance.
 */

static t_simple_cons	cons = { .vga = 0 };

/*
 * this function scrolls the screen.
 */

static void	       	cons_scroll(t_uint16			lines)
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
 * the basic print char function.
 */

static int		print_char(char			c)
{
  t_uint16		pos;

  if (cons.line >= CONS_LINES)
    cons_scroll(1);
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

  if (cons.column >= CONS_COLUMNS)
    {
      cons.column = 0;
      ++cons.line;
      if (cons.line >= CONS_LINES)
	cons_scroll(1);
    }

  pos = cons.line * CONS_COLUMNS * CONS_BPC + cons.column * CONS_BPC;

  cons.vga[pos] = c;
  cons.vga[pos + 1] = cons.attr;

  cons.column++;

  return (1);
}

/*
 * print a string
 */

int			print_string(const char* p)
{
  size_t		i;

  if (cons.vga == 0)
    return (-1);

  for (i = 0; p[i] != '\0'; i++)
    print_char(p[i]);

  return (i - 1);
}

/*
 * initialize VGA text console and console structure.
 */

static int		cons_init(void)
{
  i_region		reg;
  t_uint16		line;
  t_uint16		c;
  int			br;

  if (region_reserve(__as_id,
		     0x1000,
		     CONS_ADDR - 0x1000,
		     REGION_OPT_NONE,
		     0,
		     PAGESZ,
		     &reg) != ERROR_NONE)
    return (-1);

  cons.attr = CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_BLACK) | CONS_INT;
  cons.vga = (char*)(t_vaddr)reg;
  cons.line = 0;
  cons.column = 0;

  for (br = 1, line = CONS_LINES - 1; br && line > 0; line--)
    {
      for (c = 0; c < CONS_COLUMNS * CONS_BPC; c += CONS_BPC)
	if (cons.vga[line * CONS_COLUMNS * CONS_BPC + c * CONS_BPC])
	  {
	    br = 0;
	    break;
	  }
    }
  if (line == CONS_LINES - 1)
    {
      cons_scroll(1);
      cons.line = line;
    }
  else
      cons.line = line + 1;

  return (0);
}

/*
 * service console requests.
 */

static int		cons_serve(void)
{
  while (1)
    {
      // XXX
    }

  return (0);
}

/*
 * driver's main function.
 */

int			main(void)
{
  if (cons_init())
    return (-1);

  print_string("[teton] initialised\n");

  if (cons_serve())
    return (-1);

  return 0;
}
