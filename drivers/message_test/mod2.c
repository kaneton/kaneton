#include <klibc.h>
#include <kaneton.h>

t_uint32		syscall_async_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size);
t_uint32		syscall_sync_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size);
t_uint32		syscall_async_recv(t_uint32 tag, void* ptr, t_uint32 size);
t_uint32		syscall_sync_recv(t_uint32 tag, void* ptr, t_uint32 size);

int		main()
{
  char		recv[64];
  i_node	dest;

  while (1)
  {
    if (syscall_sync_recv(0, recv, 64) == ERROR_NONE)
      break;
  }

  dest.machine = 2^64;
  dest.task = 0;

  syscall_async_send((t_uint32 *)&dest, 0, recv, strlen(recv) + 1);

  while (1)
    ;
}
