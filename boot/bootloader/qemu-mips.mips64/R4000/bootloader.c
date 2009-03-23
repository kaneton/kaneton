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
 * updated          [mon mar 23 07:47:08 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

extern char*	flag_address;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * a funny function which does nothing.
 *
 * this function is called when a fatal error occurs.
 */

void		bootloader_error(void)
{
  while (1)
    ;
}

void		kernel_move(char* kernel_src, char* kernel_dest)
{
  while(!((*kernel_src == 0x07) && (*(kernel_src + 1) == 0x07)
	  && (*(kernel_src + 2) == 0x07) && (*(kernel_src + 3) == 0x07)))  
    *kernel_dest++ = 0x08;//*kernel_src++;*/
}

/*
 * the bootloader entry point.
 *
 * steps:
 *
 * 1) check the bootloader end flag
 * 2) flush the TLB
 * 3) active the 64 bits mode for all memory spaces
 * 4) set the EXL bit in the status register to switch the 
 *    microprocessor to kernel mode
 * 5) move the kernel to this destination KERNEL_BASE_ADDRESS
 */

void		bootloader(void)
{
  char*	kernel_dest = (unsigned char*)KERNEL_BASE_ADDRESS;
  char*	kernel_src = 0;

  /*
   * 1)
   */

  if(*flag_address != 0x07)
    bootloader_error();

  kernel_src = flag_address + 4;

  /*
   * 2)
   */

  /*
   * 3)
   */

  set_page_size(PAGE_SIZE);

  kernel_mem_space_64();
  supervisor_mem_space_64();
  user_mem_space_64();
  //while(1);

  /*
   * 4)
   */

  /*
   * 5)
   */

  kernel_move(kernel_src, kernel_dest);

  while(1)
    ;
}
