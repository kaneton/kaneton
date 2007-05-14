
#include <kaneton.h>
#include <klibc.h>

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

t_error			map_reserve(i_as		asid,
				    t_opts		opts,
				    t_vsize		size,
				    t_perms		perms,
				    t_vaddr*		addr)
{
  i_node		dest;
  o_message		message;

  dest.machine = 2^64;
  dest.task = 0;

  message.id = 0;
  message.node.machine = dest.machine;
  message.node.task = 1;

  message.u.request.operation = INTERFACE_OP_MAP_RESERVE;
  message.u.request.capability.object = asid;
  message.u.request.u.map_reserve.arg1 = opts;
  message.u.request.u.map_reserve.arg2 = size;
  message.u.request.u.map_reserve.arg3 = perms;

  syscall_async_send((t_uint32*)&dest, 0, &message, sizeof (message));

  while (syscall_async_recv(0, &message, sizeof (message)) != ERROR_NONE)
    ;

  if (message.u.reply.error != ERROR_NONE)
    return (ERROR_UNKNOWN);

  *addr = message.u.reply.u.map_reserve.result1;

  return (ERROR_NONE);
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

  if (map_reserve(1, MAP_OPT_NONE, PAGESZ, PERM_READ | PERM_WRITE, &addr) == ERROR_NONE)
    printf("map_reserve = 0x%p\n", addr);

  while (1)
    ;

  return 0;
}
