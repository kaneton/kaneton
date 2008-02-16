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
	outb $0x00, $COM1 + $1	/* IER : Disable interrupts */
	outb $0x80, $COM1 + $3	/* LCR: Enable DLAB (set baud rate divisor) */
	outb $0x0C, $COM1 + $0	/* DLL: Set divisor LSB (set divisor to 12) */
	outb $0x00, $COM1 + $1	/* DLM: Set divisor MSB */
	outb $0x03, $COM1 + $3	/* LCR: Disable DLAB,
				   set 8 data bits, no parity transmission */
	outb $0xC7, $COM1 + $2	/* ???: Enable FIFO, clear them,
				   with 14-byte treshold*/
	outb $0x0B, $COM1 + $4	/* MCR: IRQ enabled, RTS/DSR set */
	ret

/*
 * __serial32_dump
 *
 * desc   : dump bytes to serial port
 * input  : %al, byte to dump
 * output : nothing
 */
__serial32_dump:
	push %bl
1:	inb $COM1 + $5, %bl	/* LSR */
	andb $0x20, %bl		/* Get THRE bit */
	jz 1b			/* Spin until THR is ready to accept byte */
	outb $COM1, %al		/* Send %al throuh THR */
	pop %bl
	ret

/*
 * __serial32_strdump
 *
 * desc   : dump a string to serial port
 * input  : %si, pointer to string dump
 * output : nothing
 */
__serial32_strdump:
	push %al
	cld
1:
	lodsb
	test %al, %al
	jz 2f
	call __serial32_dump
	jmp 1b
2:
	pop %al
	ret

