;;
;; ---------- functions -------------------------------------------------------
;;

;;                                                                     [cut] k0

[BITS 16]			; 16-bit

[ORG 0x7c00]			; tells the assembler where to start the code

jmp main			; jump to the main program

%define ELF_SEGMENT	0x2000	; defines ELF location

;;
;; PRINT STRING
;;

print_string_real:
	mov ah, 0x0e		; the function to call with int 0x10
	mov bx, 0x07		; the console attributes

.loop:
	lodsb			; load a character from esi into al
	cmp al, 0		; is the string finished?
	je .done		; if true jump to the end

	int 0x10		; ask the BIOS to perform the task

	jmp .loop		; loop for the next character

.done:
	ret			; return 'cause the string is now displayed

;;
;; FLOPPY READ
;;

floppy_read:
	xor ax, ax		; to reinitialise the floppy controller
	mov dl, [bootdrive]	; the drive we are booting from

.init:
	int 0x13		; ask the BIOS to perform the task
	jc .init		; jump while the floppy controller is not OK

	mov ax, ELF_SEGMENT
	mov es, ax		; segment location
	xor bx, bx		; offset location

	mov ah, 0x2		; the function to call with int 0x13
	mov al, 32		; the number of sectors to read
	mov ch, 0x0		; the track to read
	mov cl, 0x2		; the sector to read
	mov dh, 0x0		; the head to read
	mov dl, [bootdrive]	; the drive to read
.retry:
	int 0x13
	jc .retry

	ret

;;
;; PROTECTED MODE ENABLE
;;

pmode_enable:
	cli			; disable interrupts

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
	mov [bootdrive], dl	; save the bootdrive identifier
	xor ax, ax		; initialise ax
	mov ds, ax		; initialise ds

	cli
	mov ss, ax		; initialise ss
	mov sp, 0xffff		; initialise the stack pointer
	sti

	mov si, newline
	call print_string

	mov si, rmode_message
	call print_string_real

	mov si, load_message
	call print_string_real

	call floppy_read	; read the ELF from the floppy

	call pmode_enable	; enable the protected mode

				; once the protected mode is enabled
				; the function pmode_main will be launched

[BITS 32]

;;
;; PRINT STRING
;;

print_string:
	mov edi, [cons_ptr]	; get current position

.loop:
	mov al, [esi]
	inc esi
	cmp al, 0		; is the string finished?
	je .done		; if true jump to the end

	cmp al, 10		; handle '\n'
	jne .next1
	add dword [cons_ptr], 160	; jump 80 characters forward
	jmp .loop

.next1:
	cmp al, 13		; handle '\r'
	jne .next2
	and dword [cons_ptr], 160	; align on 80 characters value
	jmp .loop

.next2:
	mov [edi], al		; put the character
	mov byte [edi+1], 0xf	; put attributes
	add edi, 2

	jmp .loop

.done:
	mov [cons_ptr], edi	; store back new position
	ret

;;
;; MEMCPY
;;
;; eax:	source pointer
;; ebx:	destination pointer
;; ecx:	number of byte
;;

memcpy:
	test ecx, ecx
	jnz .go
	ret
.go
	push edx
	add ecx, ebx
.loop
	mov edx, [eax]
	inc eax
	mov [ebx], edx
	inc ebx
	cmp ebx, ecx
	jnz .loop
	pop edx
	ret

;;
;; MEMSET
;;
;; eax:	destination pointer
;; ebx:	number of byte to set to zero
;;

memset_to_zero:
	test ecx, ecx
	jnz .go
	ret
.go
	push ecx
	mov ecx, eax
	add ecx, ebx
.loop
	mov byte [eax], 0
	inc eax
	cmp eax, ecx
	jnz .loop
	pop ecx
	ret

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

	mov esi, pmode_message
	call print_string

	; load code segment from ELF
	mov edx, ELF_SEGMENT*16	; the PHDR pointer offset
	add edx, [edx+0x1c]	; move into edx the PHDR offset in the ELF file

	; get offset, load address and size
	mov eax, [edx+4]	; offset
	add eax, ELF_SEGMENT*16
	mov ebx, [edx+8]	; load address
	mov ecx, [edx+16]	; size

	call memcpy		; copy code from file to runtime location

	; load data segment, allocate bss space
	add edx, 32		; goto next segment block

	; get offset, load address and size
	mov eax, [edx+4]	; offset
	add eax, ELF_SEGMENT*16
	mov ebx, [edx+8]	; load address
	mov ecx, [edx+16]	; size
	mov esi, [edx+20]	; total size in memory

	push ebx
	push ecx
	call memcpy		; copy the data from file to run location
	pop ecx
	pop ebx

	; any bss to allocate
	mov eax, esi		; compute difference between memory image
	sub eax, ecx		; size and file image size
	test eax, eax
	jz .noalloc		; if non null, the memory needs to be reset
				; (corresponding to the BSS)

	add ebx, ecx		; compute the address of the BSS
	xor eax, ebx
	xor ebx, eax
	xor eax, ebx

	call memset_to_zero	; memset with zeros

.noalloc:
	mov ebx, ELF_SEGMENT*16	; the ELF entry point offset
	mov edx, [ebx+24]	; move into edx the entry point address of
				; the ELF previously read from the floppy

	jmp edx			; jump to the ELF entry point we got earlier

				; after all the ELF is launched

;;                                                                    [cut] /k0

;;
;; DATA
;;

newline		db	10, 0

rmode_message	db	'[+] real mode', 13, 10, 0
load_message	db	'[+] loading kernel', 13, 10, 0
pmode_message	db	'[+] pmode', 13, 10, 0

bootdrive	db	0
cons_ptr	dd	0xb8000

;;                                                                     [cut] k0

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

;;                                                                    [cut] /k0
