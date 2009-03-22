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
 * updated          [sun mar 22 19:39:55 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

extern unsigned int*	flag_address;

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
 */

void		bootloader(void)
{
  unsigned int*	kernel_dest = (unsigned int*)KERNEL_BASE_ADDRESS;
  unsigned int*	kernel_src = 0;
  int i = 0;
  /*
   * 1)
   */

  if(*flag_address != 0x07070707)
    bootloader_error();

  kernel_src = flag_address + 1;

  /*
   * 2)
   */

  /*
   * 3)
   */

  kernel_mem_space_64();
  supervisor_mem_space_64();
  user_mem_space_64();
  //while(1);
  /*
   * 4)
   */

  while(*kernel_src != 0x07070707)
    {
      *kernel_dest++ = 0x07070707;
      ++i;
    }

  while(1)
    ;
}
