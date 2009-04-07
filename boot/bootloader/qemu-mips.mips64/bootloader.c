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
 * updated          [sat apr  4 07:50:31 2009]
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
 * 2) set the real kernel mode
 * 3) disable the bootstrap mode
 * 4) set the page size in the page mask register
 * 5) active the 64 bits mode for all memory spaces
 * 6) initialize the kernel and the init structure
 * 7) init the kernel stack
 * 8) jump to the kernel
 */

void		bootloader(void)
{
  t_uint64*	kernel_entry	= 0;
  void		(*kernel)(t_init*);
  t_init*	init		= NULL;

  /*
   * 1)
   */

  //debug_register(flag_address);

  if(*flag_address != BOOTLOADER_FLAG)
    bootloader_error();

  /*
   * 2)
   */

  set_kernel_mode();

  /*
   * 3)
   */

  disable_bootstrap_mode();

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

  init = bootloader_init_relocate((t_vaddr)flag_address + 4);

  /*
   * 7)
   */

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
