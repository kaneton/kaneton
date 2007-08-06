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
 * updated       matthieu bucchianeri   [tue aug  7 00:11:36 2007]
 */

#include <libc.h>
#include <crt.h>
#include <libkaneton.h>
#include "cons-simple.h"
#include "cons-simple-driver.h"

/*
 * console instance.
 */

static t_simple_cons	cons;

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
  printf_init(print_char, NULL);

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

  printf(" -- cons-simple: Simple Console driver started.\n");

  return (0);
}

/*
 * service console requests.
 */

static int		cons_serve(void)
{
  t_driver_cons_simple_message*	message;
  t_driver_cons_simple_size	count, i;
  i_node			sender;
  t_vsize			size;

  if ((message = malloc(sizeof (*message) + CONS_SIMPLE_MAX_BUFFER)) == NULL)
    {
      printf(" -- cons-simple: memory exhausted\n");
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
	      count = message->u.request.u.write.count;
	      message->u.reply.u.write.wrote = 0;
	      for (i = 0 ; i < count; i++)
		message->u.reply.u.write.wrote += print_char(message->u.request.u.write.c[i]);

	      if (message_send(sender,
			       MESSAGE_TYPE_DRIVER_CONS_SIMPLE,
			       (t_vaddr)message,
			       sizeof (*message)) != ERROR_NONE)
		{
		  /* XXX fatal */
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
  if (cons_init())
    return (-1);

  if (cons_serve())
    return (-1);

  return 0;
}
