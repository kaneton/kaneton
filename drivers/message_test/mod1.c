#include <klibc.h>
#include <kaneton.h>

t_uint32		syscall_async_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size);
t_uint32		syscall_sync_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size);
t_uint32		syscall_async_recv(t_uint32 tag, void* ptr, t_uint32 size);
t_uint32		syscall_sync_recv(t_uint32 tag, void* ptr, t_uint32 size);

int		main()
{
  char		*to_send = "ping";
  i_node	dest;

  dest.machine = 2^64;
  dest.task = 2;
  syscall_sync_send((t_uint32 *)&dest, 0, to_send, 5);

  dest.task = 1;
  syscall_sync_send((t_uint32 *)&dest, 0, to_send, 5);

  while (1)
    ;
}
