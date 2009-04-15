/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...n/boot/bootloader/qemu-mips.mips64/mmu.c
 *
 * created       enguerrand raymond   [mon mar 23 07:36:48 2009]
 * updated       enguerrand raymond   [sat apr 11 02:38:36 2009]
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "libmips64.h"

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * set the page mask
 */

/*
 * set the mask register to 0 for 4k size page
 */

void	set_page_size(t_uint32 page_mask)
{
  __asm__(".set noat\n\t"
	  "mtc0 $0, $5\n\t"
	  :
	  :"r"(page_mask)
	  );
}

/*
 * flushes the TLB. This function is used during bootstrap
 * operations
 */

void	tlb_flush(void)
{
}

/*
 * flushes the primary data cache. This function is used during
 * bootstrap operations
 */

void	cache_flush(void)
{
}

/*
 * activate the 64 bits mode for kernel memory space
 * by setting the KX bit of the status register
 */

void	kernel_mem_space_64(void)
{
  set_status_register(KX_SET);
}

/*
 * activate the 64 bits mode for supervisor memory space
 * by setting the SX bit of the status register
 */

void	supervisor_mem_space_64(void)
{
  set_status_register(SX_SET);
}

/*
 * activate the 64 bits mode to user memory space
 * by setting the UX bit of the status register
 */

void	user_mem_space_64(void)
{
  set_status_register(UX_SET);
}
