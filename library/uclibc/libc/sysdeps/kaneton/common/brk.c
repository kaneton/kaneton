#include <sys/syscall.h>
#include <unistd.h>

/* This must be initialized data because commons can't have aliases.  */
void *__curbrk attribute_hidden = 0;

#define __NR___libc_brk __NR_brk
_syscall1(int, __libc_brk, void*, increment);
libc_hidden_proto(brk)
weak_alias(__libc_brk,brk)
libc_hidden_weak(brk)
