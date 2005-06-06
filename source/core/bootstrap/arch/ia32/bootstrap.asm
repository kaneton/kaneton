[BITS 16]			; 16-bit

[ORG 0x7c00]			; tells the compiler where to start the code

jmp main

;; PRINT_STRING

XXXprint_string:
	mov ah, 0x0e
	mov bx, 0x0f

print_string_loop:
	;; 	loadsb			; load a character from esi into al

	cmp al, 0
	je print_string_done

	int 0x10

	jmp print_string_loop

print_string_done:
	ret

print_string:
	mov ah, 0x0e
	mov bx, 0x0f
	loadsb
	int 0x10
	ret

;; MAIN

main:
	mov si, bootstrap_message
	call print_string

;; IDLE

idle_loop:
	jmp idle_loop

;; DATA

bootstrap_message db	'bootstrap',13,10,0

;; BOOTLOADER SIGNATURE

times 510-($-$$) db 0		; fill the rest of the sector with zeros
dw 0xAA55			; add the boot loader signature to the end
