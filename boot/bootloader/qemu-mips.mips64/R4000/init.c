/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...bootloader/qemu-mips.mips64/R4000/init.c
 *
 * created          [thu apr  2 14:39:33 2009]
 * updated          [fri apr  3 16:58:01 2009]
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * This function calculates the kernel size between the
 * two flags
 */

t_size		kernel_size(t_vaddr	kernel_address)
{
  t_uint8*	ptr = (t_uint8*)kernel_address;
  
  while(!((*ptr == 0x07) && (*(ptr + 1) == 0x07)
	  && (*(ptr + 2) == 0x07) && (*(ptr + 3) == 0x07)))
    ++ptr;

  return ((t_vaddr)ptr - 1) - kernel_address;
}

/*
 * steps:
 *
 * 1) Copy the kernel to this final location
 * 2) 
 */

t_vaddr		bootloader_kernel_init(t_vaddr kernel_address)
{
  t_size	k_size = 0;
  t_init*	init = (t_init*)0;

  /*
   * 1)
   */

  k_size = kernel_size(kernel_address);

  memcpy((t_uint8*)KERNEL_BASE_ADDRESS, (t_uint8*) kernel_address, k_size);
  return (t_vaddr)init;
}
