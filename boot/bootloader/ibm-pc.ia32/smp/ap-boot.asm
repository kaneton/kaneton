;;
;; This code is for Application Processor (secondary processor) bootstrap.
;;
;; This code must be loaded at address 0x8000.
;; Address 0x7FF6 must be loaded with a valid GDTR value.
;; Address 0x7FFC must contain a function pointer to the high-level
;; bootstrap routine.
;;

;; When compiled, this code is placed into an unknown location in the
;; final ELF file. These two symbols are used to retrieve this
;; location and the size of the code.

global ap_boot_start
global ap_boot_end

[BITS 16]                       ; 16-bit mode

;;
;; PROTECTED MODE ENABLE
;;

ap_boot_start:
	cli			; disable interrupts

	mov esi, 0x7FF6
	lgdt [esi]		; load the gdtr new value

	mov eax, cr0		; get cr0
	or ax, 1		; set the protected mode bit
	mov cr0, eax		; set cr0

	jmp 0x8:0x8019		; jump into the segment 0x8

[BITS 32]			;  32-bit mode.

;;
;; JUMP TO THE ADVANCED AP INIT
;;
;; here is 0x8019
	mov ax, 0x10		; represent the data segment selector
	mov ds, ax		; set the data segment register
	mov ss, ax		; set the stack segment register
	mov es, ax		; set the es register
	mov fs, ax		; set the fs register
	mov gs, ax		; set the gs register
	mov esp, 0x9000		; reinitialize the stack pointer

	mov esi, 0x7FFC		; call the kernel ap init
	mov edx, [esi]		; procedure
	jmp edx
ap_boot_end:
