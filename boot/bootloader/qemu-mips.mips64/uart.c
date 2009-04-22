/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/.../boot/bootloader/qemu-mips.mips64/uart.c
 *
 * created       enguerrand raymond   [wed apr 22 06:14:28 2009]
 * updated       enguerrand raymond   [wed apr 22 08:26:02 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "uart.h"

/*
 * ---------- functions -------------------------------------------------------
 */

void		uart_init(void)
{
  char*	uart_mcr = (char*)UART_BASE + UART_MCR;
  char*	uart_lcr = (char*)UART_BASE + UART_LCR;
  char* uart_dll = (char*)UART_BASE + UART_DLL;
  char*	uart_dlm = (char*)UART_BASE + UART_DLM;
  
  *uart_mcr = UART_MCR_ON_1;
  *uart_lcr = UART_LCR_DLAB;

  *uart_dll = 10;
  *uart_dlm = 0;

  *uart_lcr = UART_LCR_W;
}

void	write_char(char c)
{
  char*	uart_data = (char*)UART_BASE + UART_DATA;
  char*	uart_lsr = (char*)UART_BASE + UART_LSR;

  *uart_data = c;

  while((*uart_lsr & UART_LSR_TE) == 0)
    ;
}
