#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <libkaneton.h>
#include <crt.h>

void* __libc_mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset)
{
  t_vaddr	addr;

  if (!(flags & MAP_ANONYMOUS))
    return (MAP_FAILED);

  if (length % 4096)
    length = ((length / 4096) + 1) * 4096;

  if (map_reserve(_crt_get_as_id(), MAP_OPT_NONE, length, PERM_READ | PERM_WRITE, &addr) != ERROR_NONE)
    return (MAP_FAILED);

  return (void*)addr;
}

libc_hidden_proto(mmap)
weak_alias(__libc_mmap,mmap)
libc_hidden_weak(mmap)
