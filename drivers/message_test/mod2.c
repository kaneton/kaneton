#include <libc.h>
#include <kaneton.h>
#include "common.h"

int		main()
{
  char		recv[64];
  i_node	dest;

  if (syscall_sync_recv(0, recv, 64, FLAG_BLOCKING) != ERROR_NONE)
    memcpy(recv, ":(", 3);

  dest.machine = 2^64;
  dest.task = 0;

  syscall_async_send((t_uint32 *)&dest, 0, recv, strlen(recv) + 1);

  while (1)
    ;
}
