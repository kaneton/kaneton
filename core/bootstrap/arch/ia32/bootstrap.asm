[BITS 16]			; 16-bit

[ORG 0x7c00]			; tells the compiler where to start the code

jmp main			; jump to the main program

%define ELF_SEGMENT	0x2000	; defines ELF location

;;
;; PRINT STRING
;;

print_string:
	mov ah, 0x0e		; the function to call with int 0x10
	mov bx, 0x07		; the console attributes

print_string_loop:
	lodsb			; load a character from esi into al
	cmp al, 0		; is the string finished?
	je print_string_done	; if true jump to the end

	int 0x10		; ask the BIOS to perform the task

	jmp print_string	; loop for the next character

print_string_done:
	ret			; return 'cause the string is now displayed

;;
;; FLOPPY READ
;;

floppy_read:
	xor ax, ax		; to reinitialise the floppy controller

floppy_read_init:
	int 0x13		; ask the BIOS to perform the task
	jc floppy_read_init	; jump while the floppy controller is not OK

	mov ax, ELF_SEGMENT
	mov es, ax		; segment location
	xor bx, bx		; offset location

	mov ah, 0x2		; the function to call with int 0x13
	mov al, 0x1		; the number of sectors to read
	mov ch, 0x0		; the track to read
	mov cl, 0x2		; the sector to read
	mov dh, 0x0		; the head to read
	mov dl, 0x0		; the drive to read

floppy_read_retry:
	int 0x13
	jc floppy_read_retry

floppy_read_done:
	ret

;;
;; PROTECTED MODE ENABLE
;;

pmode_enable:
	cli			; disable interrupts

	mov bx, 24		; the ELF entry point offset
	mov dword edx, [es:bx]	; move into edx the entry point address of
				; the ELF previously read from the floppy

	mov ax, eogdt		; the address of the end of the GDT
	mov bx, gdt		; the address of the start of the GDT
	sub ax, bx		; compute the GDT size

	mov [gdtr + 0], ax	; set the GDT size into the gdtr

	mov eax, gdt		; the address of the GDT
	mov [gdtr + 2], eax	; set the GDT address into the gdtr

	lgdt [gdtr]		; load the GDT using the gdtr

	mov eax, cr0		; get the cr0
	or ax, 1		; set the protected mode bit
	mov cr0, eax		; set the cr0

	jmp 0x8:pmode_main	; jump into the segment 0x8 to the pmode_main
				; function to continue the execution

;;
;; MAIN
;;

main:
	xor ax, ax		; initialise ax
	mov ds, ax		; initialise ds

	cli
	mov ss, ax		; initialise ss
	mov sp, 0xffff		; initialise the stack pointer
	sti

	mov si, newline
	call print_string

	mov si, rmode_message
	call print_string

	call floppy_read	; read the ELF from the floppy

	call pmode_enable	; enable the protected mode

				; once the protected mode is enabled
				; the function pmode_main will be launched

[BITS 32]

;;
;; PROTECTED MODE MAIN
;;

pmode_main:
	mov ax, 0x10		; represent the data segment selector
	mov ds, ax		; set the data segment register
	mov ss, ax		; set the stack segment register
	mov es, ax		; set the es register
	mov fs, ax		; set the fs register
	mov gs, ax		; set the gs register
	mov esp, 0x9000		; reinitialise the stack pointer

	jmp edx			; jump to the ELF entry point we got earlier

				; after all the ELF is launched

;;
;; DATA
;;

newline		db	10, 0

rmode_message	db	'[+] real mode', 13, 10, 0

pmode_message	db	'[+] protected mode', 13, 10, 0

gdt:
gdt_null:
		db	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
gdt_cs:
		db	0xff, 0xff, 0x0, 0x0, 0x0, 10011011b, 11011111b, 0x0
gdt_ds:
		db	0xff, 0xff, 0x0, 0x0, 0x0, 10010011b, 11011111b, 0x0
eogdt:

gdtr:
		dw	0x0
		dd	0x0

;;
;; BOOTLOADER SIGNATURE
;;

times 510-($-$$) db 0		; fill the rest of the sector with zeros
dw 0xAA55			; add the bootloader signature to the end
