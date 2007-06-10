#include <libc.h>
#include <kaneton.h>
#include "common.h"

int		main()
{
  char		*to_send = "ping";
  i_node	dest;

  int i;

  for (i = 0; i < 9999999; i++);

  dest.machine = 2^64;
  dest.task = 2;
  syscall_sync_send((t_uint32 *)&dest, 0, to_send, 5);

  while (1)
    ;
}
