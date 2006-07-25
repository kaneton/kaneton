[BITS 16]                       ; 16-bit mode

;;
;; quick map of the memory
;; 0x7FFC:	core fun ptr
;; 0x8000:	code
;;

[ORG 0x8000]

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

[BITS 32]			;  32-bit mode.

;;
;; JUMP TO THE ADVANCED AP INIT
;;

pmode_main:
	mov ax, 0x10		; represent the data segment selector
	mov ds, ax		; set the data segment register
	mov ss, ax		; set the stack segment register
	mov es, ax		; set the es register
	mov fs, ax		; set the fs register
	mov gs, ax		; set the gs register
	mov esp, 0x9000		; reinitialise the stack pointer

	mov esi, 0x7FFC		; call the kernel ap init
	mov edx, [esi]		; procedure
	jmp edx

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
