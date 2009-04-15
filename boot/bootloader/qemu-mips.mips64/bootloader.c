/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...bootloader/qemu-mips.mips64/bootloader.c
 *
 * created          [sun feb  8 17:24:44 2009]
 * updated          [fri apr 10 14:49:31 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

extern t_uint32*	flag_address;

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
 * 2) enable coprocessor 0
 * 3) set the page size in the page mask register
 * 4) active the 64 bits mode for all memory spaces
 * 5) initialize the kernel and the init structure
 * 6) set the real kernel mode
 * 7) disable the bootstrap mode
 * 8) init the kernel stack
 * 9) jump to the kernel
 */

void		bootloader(void)
{
  t_uint64*	kernel_entry	= 0;
  void		(*kernel)(t_init*);
  t_init*	init		= NULL;

  /*
   * 1)
   */

  if(*flag_address != BOOTLOADER_FLAG)
    bootloader_error();

  /*
   * 2)
   */

  /*
   * 3)
   */

  set_page_size(PAGE_SIZE);

  /*
   * 4)
   */

  kernel_mem_space_64();
  supervisor_mem_space_64();
  user_mem_space_64();

  /*
   * 5)
   */

  init = bootloader_init_relocate((t_vaddr)(flag_address + 1));

  /*
   * 6)
   */

  set_kernel_mode();

  /*
   * 7)
   */

  disable_bootstrap_mode();

  /*
   * 8)
   */

  /*
   * 9)
   */

  kernel_entry = (t_uint64*)(KERNEL_BASE_ADDRESS + 0x18);

  kernel = (void (*)(t_init*))(*kernel_entry);

  kernel(init);
  
  while(1)
    ;
}
