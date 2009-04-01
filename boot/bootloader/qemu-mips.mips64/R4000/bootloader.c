/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...ader/qemu-mips.mips64/R4000/bootloader.c
 *
 * created          [sun feb  8 17:24:44 2009]
 * updated          [wed apr  1 09:49:45 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

extern int*	flag_address;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * This function moves the kernel to this final destination
 */

void		kernel_move(char* kernel_src, char* kernel_dest)
{
  while(!((*kernel_src == 0x07) && (*(kernel_src + 1) == 0x07)
	  && (*(kernel_src + 2) == 0x07) && (*(kernel_src + 3) == 0x07)))
    *kernel_dest++ = *kernel_src++;
}

/*
 * A funny function which does nothing.
 *
 * This function is called when a fatal error occurs.
 */

void		bootloader_error(void)
{
  while (1)
    ;
}

/*
 * The bootloader entry point.
 *
 * steps:
 *
 * 1) Check the bootloader end flag
 * 2) Flush the TLB
 * 3) Prepare the cache
 * 4) Set the page size in the page mask register
 * 5) Active the 64 bits mode for all memory spaces
 * 6) Move the kernel to this destination KERNEL_BASE_ADDRESS
 */

void		bootloader(void)
{
  char*	kernel_dest = (char*)KERNEL_BASE_ADDRESS;
  char*	kernel_src = 0;

  /*
   * 1)
   */

  if(*flag_address != 0x07070707)
    bootloader_error();

  kernel_src = ((char*)(flag_address)) + 4;

  /*
   * 2)
   */

  /*
   * 3)
   */

  /*
   * 4)
   */

  set_page_size(PAGE_SIZE);

  /*
   * 5)
   */

  kernel_mem_space_64();
  supervisor_mem_space_64();
  user_mem_space_64();

  /*
   * 6)
   */

  kernel_move(kernel_src, kernel_dest);

  while(1)
    ;
}
