.globl _start

.text
.code32

.ascii "This string is here to shift the _start symbol\r\n\0"

_start:
  push %eax
  push %ebx
  jmp kernel_main
1:
  jmp 1b

