#include <sys/syscall.h>
#include <unistd.h>

#include <crt.h>
#include <libkaneton.h>
#define CONS_SIMPLE_SPAWN_INTERFACE
#include "../../../../../../drivers/cons-simple/cons-simple-driver.h"
#define MOD_SPAWN_INTERFACE
#include "../../../../../../services/mod/mod-service.h"

static i_task	cons = ID_UNUSED;

extern __typeof(write) __libc_write;
#define __NR___libc_write __NR_write
ssize_t __libc_write(int fd, const __ptr_t buf, size_t count)
{
  t_uint32	wrote;

  if (fd != 1)
    return (-1);

  if (cons == ID_UNUSED)
    {
      if (_crt_get_mod_id() == ID_UNUSED ||
	  mod_get_service(_crt_get_mod_id(),
			  DRIVER_CONS_SIMPLE_NAME, &cons) != ERROR_NONE)
	return (count);

      cons_simple_init();
    }

  cons_simple_write(cons, count, buf, &wrote);

  return (wrote);
}

libc_hidden_proto(write)
weak_alias(__libc_write,write)
libc_hidden_weak(write)
