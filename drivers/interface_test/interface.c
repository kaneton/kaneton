#include <kaneton.h>

#include <klibc.h>
#include "interface_user.h"

t_uint32		syscall_async_send(t_uint32*		node,
					   t_uint32		tag,
					   void*		ptr,
					   t_uint32		size)
{
  t_uint32		ret;

  asm volatile("	pushl %%ebp		\n"
	       "	movl %7, %%ebp		\n"
	       "	int $56			\n"
	       "	popl %%ebp		"
	       : "=a" (ret)
	       : "a" (node[0]), "b" (node[1]), "c" (node[2]), "d" (node[3]),
	       "S" (tag), "D" (ptr), "m" (size));

  return (ret);
}


t_uint32		syscall_async_recv(t_uint32		tag,
					   void*		ptr,
					   t_uint32		size)
{
  t_uint32		ret;

  asm volatile("	int $57			"
	       : "=a" (ret)
	       :"a" (tag), "b" (ptr), "c" (size));

  return (ret);
}

t_cons			cons;

int			cons_print_char(char			c)
{
  t_uint16		pos;

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
    }

  pos = cons.line * CONS_COLUMNS * CONS_BPC + cons.column * CONS_BPC;

  cons.vga[pos] = c;
  cons.vga[pos + 1] = cons.attr;

  cons.column++;

  return (1);
}


int			main(void)
{
  cons.attr = CONS_FRONT(CONS_WHITE) | CONS_BACK(CONS_RED) | CONS_INT;
  cons.vga = (char*)CONS_ADDR;
  cons.line = 0;
  cons.column = 0;
  printf_init(cons_print_char, NULL);
  printf("chiche\n");

  t_vaddr		addr;

  if (syscall_map_reserve(1, MAP_OPT_NONE, PAGESZ, PERM_READ | PERM_WRITE, &addr) == ERROR_NONE)
    printf("map_reserve = 0x%p\n", addr);
  else
    printf("error map_reserve\n");

  while (1)
    ;

  return 0;
}
