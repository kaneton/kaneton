#include <features.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

libc_hidden_proto(_exit)
void attribute_noreturn _exit(int status)
{
  write(1, "exit\n", 5);
	/* The loop is added only to keep gcc happy. */
	while(1)
	  ;
}
libc_hidden_def(_exit)
