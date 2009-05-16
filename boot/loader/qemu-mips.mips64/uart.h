/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/kaneton/boot/loader/qemu-mips.mips64/uart.h
 *
 * created       enguerrand raymond   [wed apr 22 06:11:59 2009]
 * updated       enguerrand raymond   [wed may 13 08:40:00 2009]
 */

#ifndef BOOTLOADER_UART_H
#define BOOTLOADER_UART_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define UART_BASE       0xffffffffb40003f8
#define UART_DATA       0x0
#define UART_DLL        0x0
#define UART_DLM        0x1
#define UART_IER        0x1
#define UART_IIR        0x2
#define UART_FCR        0x2
#define UART_LCR        0x3
#define UART_MCR        0x4
#define UART_LSR        0x5
#define UART_MSR        0x6
#define UART_SCR        0x7


#define UART_LCR_W      0x07    // Set UART to 8,N,2 & DLAB = 0
#define UART_LCR_DLAB   0x87    // Set UART to 8,N,2 & DLAB = 1
#define UART_DLL_96     0x78    // Sel UART Divisor latch LSB to 120 Dec
#define UART_DLM_96     0x00    // Sel UART Divisor latch MSB to 0
#define UART_MCR_ON     0x0f    // Turn on LEDS, DTR and DSR
#define UART_MCR_OFF    0x03    // Turn off LEDS, turn on DTR and DSR

#define UART_FCR_FIFO_EN 0x01	// FIFO enable

#define UART_LSR_DR     0x01    // UART received data present
#define UART_LSR_TE     0x40    // UART transmitter empty

#define UART_MCR_ON_1   0x04
#define UART_MCR_ON_2	0x08

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	uart.c
 */

/*
 * uart.c
 */

void		uart_init(void);

void	write_char(char c);


/*
 * eop
 */

#endif
