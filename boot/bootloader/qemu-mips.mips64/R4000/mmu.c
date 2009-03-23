/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/.../bootloader/qemu-mips.mips64/R4000/mmu.c
 *
 * created          [mon mar 23 07:36:48 2009]
 * updated          [mon mar 23 07:46:26 2009]
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "cp0.h"

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * Set the page mask
 */

/*
 * Set the mask register to 0 for 4k size page
 */

void	set_page_size(int page_mask)
{
  __asm__(".set noat\n\t"
	  "mtc0 $0, $5\n\t"
	  :
	  :"r"(page_mask)
	  );
}

/*
 * Activate the 64 bits mode for kernel memory space
 * by setting the KX bit of the status register
 */

void	kernel_mem_space_64(void)
{
  set_status_register(KX_SET);
}

/*
 * Activate the 64 bits mode for supervisor memory space
 * by setting the SX bit of the status register
 */

void	supervisor_mem_space_64(void)
{
  set_status_register(SX_SET);
}

/*
 * Activate the 64 bits mode to user memory space
 * by setting the UX bit of the status register
 */

void	user_mem_space_64(void)
{
  set_status_register(UX_SET);
}
