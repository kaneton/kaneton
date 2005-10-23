;;
;; this program just prints the file contents.
;;
;; the file is passed as argv[1].
;;
;; this program uses C library functions as direct system calls.
;; indeed, to learn assembly this program uses the two ways to force
;; students to understand both.
;;
;; the C library functions like printf() can be called without problems
;; but take care of register overwritting. the system calls must be used
;; without the C library meaning that you program must directly make the
;; interrupt.
;;
;; moreover, this work must be done using nasm and the Intel assembly
;; syntax.
;;
;; the goal of this work is not to program a perfect cat program. for this
;; reason the student does not have to handle errors.
;;
;; tips for linux operating systems:
;;
;; /usr/include/bits/syscall.h
;; /usr/include/asm/unistd.h
;;
;; /usr/include/asm-generic/errno-base.h
;;
;; to compile it:
;;
;;    nasm -f elf cat.asm -o cat.o
;;    gcc -o cat cat.o
;;

;;
;; EXTERN
;;

extern printf

;;
;; GLOBAL
;;

global main

;;
;; DIRECTIVES
;;

[BITS 32]

;;
;; ENTRY POINT
;;

jmp main			; jump on the main function

;;
;; OPEN
;;

open:
	push ebp		; build the frame
	mov ebp, esp

	mov eax, 5		; eax = SYS_open = 5
	mov ebx, [ebp + 8]	; ebx = pathname
	mov ecx, [ebp + 12]	; ecx = flags
	mov edx, [ebp + 16]	; edx = mode
	int 0x80		; call the kernel with an interrupt

	push eax
	push fd_format
	call printf		; print the file descriptor

	add esp, 4
	pop eax			; restore correct return value

	mov esp, ebp		; restore the frame
	leave			; destroy the frame
	ret			; return

;;
;; READ
;;

read:
	push ebp		; build the frame
	mov ebp, esp

	mov eax, 3		; eax = SYS_read = 3
	mov ebx, [ebp + 8]	; ebx = fd
	mov ecx, [ebp + 12]	; ecx = buf
	mov edx, [ebp + 16]	; edx = count
	int 0x80		; call the kernel with an interrupt

	mov esp, ebp		; restore the frame
	leave			; destroy the frame
	ret			; return

;;
;; WRITE
;;

write:
	push ebp		; build the frame
	mov ebp, esp

	mov eax, 4		; eax = SYS_write = 4
	mov ebx, 1		; ebx = 1 = stdout
	mov ecx, [ebp + 12]	; ecx = buf
	mov edx, [ebp + 16]	; edx = count
	int 0x80		; call the kernel with an interrupt

	mov esp, ebp		; restore the frame
	leave			; destroy the frame
	ret			; return

;;
;; CAT
;;

cat:
	push ebp		; build the frame
	mov ebp, esp
	sub esp, 4		; local variable: fd
	sub esp, 4		; local variable: buf
	sub esp, 128		; the buffer

	mov ebx, ebp
	sub ebx, 136
	mov [ebp - 8], ebx	; store the buffer address into the
				; local variable buf

	mov eax, [ebp + 8]	; eax = file

	push dword 0		; mode = do not care about
	push dword 0		; flags = RDONLY = 0
	push eax		; pathname = file
	call open		; call the open assembly function

	mov [ebp - 4], eax	; store the filedescriptor in the fd variable

while:
	mov byte [ebp - 136], 0	; buf[0] = 0

	push dword 128		; count = 128
	push dword [ebp - 8]	; buf = local variable buf
	push dword [ebp - 4]	; fd = local variable fd
	call read		; call the read assembly function

	mov edi, eax		; edi = eax = return value of read()

	push edi		; count = edi
	push dword [ebp - 8]	; buf = local variable buf
	push dword [ebp - 4]	; fd = local variable fd
	call write		; call the write assembly function

	cmp edi, 0		; if (edi != 0)
	jne while		;   goto while

	mov esp, ebp		; restore the frame
	leave			; destroy the frame
	ret			; return

;;
;; MAIN
;;

main:
	push ebp		; build the frame
	mov ebp, esp

	mov eax, [ebp + 12]	; eax = argv
	mov eax, [eax + 4]	; eax = argv[1]

	pusha			; save the context

	push eax
	push file_format
	call printf		; call printf() to printf a message

	add esp, 8
	popa			; restore the context

	push eax		; file
	call cat		; call the cat assembly function

	mov esp, ebp		; restore the frame
	leave			; destroy the frame
	ret			; return

;;
;; DATA
;;

file_format	db		'[file] %s', 13, 10, 0
fd_format	db		'[fd] %d', 13, 10, 0
write_format	db		'%s', 13, 10, 0
