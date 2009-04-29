/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...bootloader/qemu-mips.mips64/bootloader.c
 *
 * created       enguerrand raymond   [sun feb  8 17:24:44 2009]
 * updated       enguerrand raymond   [tue apr 28 11:33:32 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

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

void		bootloader(multiboot_info_t* mbi)
{
  void		(*kaneton)(t_init*);
  t_init*	init = NULL;
  Elf64_Ehdr*	kaneton_kernel = NULL;

  /*
   * 1)
   */

  bootloader_cons_init();
  printf("\n");
  printf("                --- the kaneton microkernel project ---\n");
  printf("\n\n");

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

  init = bootloader_init_relocate(mbi);

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

  kaneton_kernel = (Elf64_Ehdr*)KERNEL_BASE_ADDRESS;

  bootloader_cons_msg('+', "Jump to the kaneton function 0x%lx\n", kaneton_kernel->e_entry);

  kaneton = (void (*)(t_init*))kaneton_kernel->e_entry;
  
  kaneton(init);

  while(1)
    ;
}
