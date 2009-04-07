/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/.../boot/bootloader/qemu-mips.mips64/init.c
 *
 * created          [thu apr  2 14:39:33 2009]
 * updated          [sun apr  5 06:22:28 2009]
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * this variable is used by the page allocator.
 *
 * it is necessary to initialize it because variable in bss
 * shift the flag and the flag address pointer is set after
 * the real flag
 */

static t_paddr		relocate = KERNEL_BASE_ADDRESS;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just allocates memory for relocation.
 *
 * this function is needed because many objects are dynamic: inputs,
 * page tables etc.
 */

t_paddr		bootloader_init_alloc(t_psize	size,
				      t_psize*	psize)
{
  t_paddr	allocated = relocate;
  t_psize	padded;

  padded = size / PAGESZ;
  padded += size % PAGESZ ? 1 : 0;

  relocate += padded * PAGESZ;

  if (psize != NULL)
    *psize = padded * PAGESZ;

  return (allocated);
}

/*
 * this function calculates the kernel size between the
 * two flags
 */

t_size		bootloader_init_kernel_size(t_vaddr	kernel_address)
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
 * 1) copy the kernel to this final location
 * 2) 
 */

t_init*		bootloader_init_relocate(t_vaddr	kernel_address)
{
  t_size	kernelsz = 0;
  t_init*	init = NULL;
  t_paddr	kcode;
  t_psize	kcodesz;
  t_psize	initsz;

  /*
   * 1)
   */

  kernelsz = bootloader_init_kernel_size(kernel_address);

  kcode = bootloader_init_alloc(kernelsz, &kcodesz);

  memcpy((t_uint8*)KERNEL_BASE_ADDRESS, (t_uint8*) kernel_address, kernelsz);

  /*
   * 2)
   */

  init = (t_init*)bootloader_init_alloc((t_psize)sizeof(t_init), &initsz);
  memset(init, 0, sizeof(t_init));
  
  /*
   * 3)
   */

  init->kcode = kcode;
  init->kcodesz = kcodesz;


  return init;
}
