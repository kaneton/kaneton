.text
.code32

#define COM1 0x3f8

/*
 * We are here programming the Intel 82C50A chip which is a programmable UART
 * attached to a typical PC RS232 port.
 *
 * IER : Interrupt Enable Register
 * LCR : Line Control Register
 * RBR : Receive Buffer register
 * THR : Transmitter Holding Register
 * DLL : Divisor Latch, Less significant byte
 * DLM : Divisor Latch, Most significant byte
 * DLAB : Divisor Latch Access Bit (0: Address 0 is mapped to the RBR/THR,
 *				       Address 1 is mapped to the IER
 *				    1: Address 0 is mapped to the DLL register,
 *				       Address 1 is mapped to the DLM register)
 * IIR : Interrupt Identification Register
 * MCR : Modem Control Register
 * LSR : Line Status Register
 * THRE : Transmitter Holding Register Empty
 */

/*
 * __serial32_init
 *
 * desc   : initializes the serial port
 * input  : nothing
 * output : nothing
 */
__serial32_init:
	push %edx

	movb $0x00, %al
	movw $COM1 + 1, %dx
	outb %al, %dx		/* IER : Disable interrupts */
	
	movb $0x80, %al
	movw $COM1 + 3, %dx
	outb %al, %dx		/* LCR: Enable DLAB (set baud rate divisor) */
	
	movb $0x0C, %al
	movw $COM1 + 0, %dx
	outb %al, %dx		/* DLL: Set divisor LSB (set divisor to 12) */
	

	movb $0x00, %al
	movw $COM1 + 1, %dx
	outb %al, %dx		/* DLM: Set divisor MSB */

	movb $0x03, %al
	movw $COM1 + 3, %dx
	outb %al, %dx		/* LCR: Disable DLAB,
				   set 8 data bits, no parity transmission */
	
	movb $0xC7, %al
	movw $COM1 + 2, %dx
	outb %al, %dx		/* ???: Enable FIFO, clear them,
				   with 14-byte treshold*/

	movb $0x0B, %al
	movw $COM1 + 4, %dx
	outb %al, %dx		/* MCR: IRQ enabled, RTS/DSR set */
	
	pop %edx
	ret

/*
 * __serial32_dump
 *
 * desc   : dump bytes to serial port
 * input  : %al, byte to dump
 * output : nothing
 */
__serial32_dump:
	push %ebx
	movb %al, %bl

1:
	mov $COM1 + 5, %dx
	inb %dx, %al		/* LSR */
	andb $0x20, %al		/* Get THRE bit */
	jz 1b			/* Spin until THR is ready to accept byte */

	movw $COM1, %dx
	movb %bl, %al
	outb %al, %dx		/* Send %al throuh THR */
	
	pop %ebx
	ret

/*
 * __serial32_strdump
 *
 * desc   : dump a string to serial port
 * input  : %si, pointer to string dump
 * output : nothing
 */
__serial32_strdump:
	cld
1:
	lodsb
	test %al, %al
	jz 2f
	call __serial32_dump
	jmp 1b
2:
	ret

