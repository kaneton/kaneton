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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crt.h>
#include <libkaneton.h>
#include "cons-simple.h"
#include "cons-simple-driver.h"

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

int			print_string(const char* p, size_t len)
{
  size_t		i;

  if (cons.vga == 0)
    return (-1);

  for (i = 0; i < len; i++)
    print_char(p[i]);

  return (len);
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

  if (region_reserve(_crt_get_as_id(),
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

  print_string(" -- cons-simple: Simple Console driver started.\n", 48);

  return (0);
}

/*
 * service console requests.
 */

static int		cons_serve(void)
{
  t_driver_cons_simple_message*	message;
  i_node			sender;
  t_vsize			size;

  if ((message = malloc(sizeof (*message) + CONS_SIMPLE_MAX_BUFFER)) == NULL)
    {
      print_string(" -- cons-simple: memory exhausted\n", 34);
      return (-1);
    }

  while (1)
    {
      if (message_receive(MESSAGE_TYPE_DRIVER_CONS_SIMPLE,
			  (t_vaddr)message,
			  &size,
			  &sender) == ERROR_NONE)
	{
	  if (message->u.request.operation == CONS_SIMPLE_DRIVER_WRITE)
	    {
	      message->u.reply.u.write.wrote = print_string(message->u.request.u.write.c, message->u.request.u.write.count);

	      if (message_send(sender,
			       MESSAGE_TYPE_DRIVER_CONS_SIMPLE,
			       (t_vaddr)message,
			       sizeof (*message)) != ERROR_NONE)
		{
		  print_string(" -- cons-simple: error in request\n", 34);
		}
	    }
	}
    }

  return (0);
}

/*
 * driver's main function.
 */

int			main(void)
{
  if (message_register(MESSAGE_TYPE_DRIVER_CONS_SIMPLE,
		       MESSAGE_SIZE_DRIVER_CONS_SIMPLE) != ERROR_NONE)
    return (-1);

  if (cons_init())
    return (-1);

  if (cons_serve())
    return (-1);

  return 0;
}
