#include <sys/syscall.h>
#include <unistd.h>
#include <sys/mman.h>
#include <libkaneton.h>
#include <crt.h>

int __libc_munmap (void *__addr, size_t __len)
{
  return 0;

  return (map_release(_crt_get_as_id(), (t_vaddr)__addr) == ERROR_NONE ? 0 : -1);
}

libc_hidden_proto(munmap)
weak_alias(__libc_munmap,munmap)
libc_hidden_def(munmap)
