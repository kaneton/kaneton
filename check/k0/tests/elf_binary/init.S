.globl _start

.text
.code32

_start:
  push %eax
  push %ebx
  jmp kernel_main
1:
  jmp 1b

