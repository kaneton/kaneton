#include <libc.h>
#include <kaneton.h>

t_uint32		syscall_async_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size)
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

t_uint32		syscall_sync_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size)
{
  t_uint32		ret;

  asm volatile("	pushl %%ebp		\n"
	       "	movl %7, %%ebp		\n"
	       "	int $58			\n"
	       "	popl %%ebp		"
	       : "=a" (ret)
	       : "a" (node[0]), "b" (node[1]), "c" (node[2]), "d" (node[3]),
	       "S" (tag), "D" (ptr), "m" (size));

  return (ret);
}


t_uint32		syscall_async_recv(t_uint32 tag, void* ptr, t_uint32 size)
{
  t_uint32		ret;

  asm volatile("	int $57			"
	       : "=a" (ret)
	       :"a" (tag), "b" (ptr), "c" (size));

  return (ret);
}

t_uint32		syscall_sync_recv(t_uint32 tag, void* ptr, t_uint32 size, t_uint32 flags)
{
  t_uint32		ret;

  asm volatile("	int $59			"
	       : "=a" (ret)
	       :"a" (tag), "b" (ptr), "c" (size), "d" (flags));

  return (ret);
}
