/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/.../boot/bootloader/qemu-mips.mips64/init.c
 *
 * created       enguerrand raymond   [thu apr  2 14:39:33 2009]
 * updated       enguerrand raymond   [sat apr 11 02:38:03 2009]
 */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function just allocates memory for relocation.
 *
 * this function is needed because many objects are dynamic: inputs,
 * page tables etc.
 */

t_paddr		bootloader_init_alloc(t_paddr*	relocate,
				      t_psize	size,
				      t_psize*	psize)
{
  t_paddr	allocated = *relocate;
  t_psize	padded;
      
  padded = size / PAGESZ;
  padded += size % PAGESZ ? 1 : 0;
      
  *relocate += padded * PAGESZ;
  
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
 * 2) create init structure
 * 3) fill first init fields (mem, kernel and init fields)
 */

t_init*		bootloader_init_relocate(t_vaddr	kernel_address)
{
  t_paddr	relocate	= KERNEL_BASE_ADDRESS;
  t_size	kernelsz	= 0;
  t_init*	init		= NULL;
  t_paddr	kcode;
  t_psize	kcodesz;
  t_psize	initsz;
  t_psize	kstacksz;
  t_psize	allocsz;

  /*
   * 1)
   */

  kernelsz = bootloader_init_kernel_size(kernel_address);

  kcode = bootloader_init_alloc(&relocate, kernelsz, &kcodesz);

  memcpy((t_uint8*)kcode, (t_uint8*)kernel_address, kernelsz);

  /*
   * 2)
   */

  init = (t_init*)bootloader_init_alloc(&relocate, sizeof(t_init), &initsz);
  memset(init, 0, initsz);

  /*
   * 3)
   */

  init->mem = 0;
  init->memsz = 0;

  init->kcode = kcode;
  init->kcodesz = kcodesz;
  
  init->init = (t_paddr)init;
  init->initsz = initsz;

  init->kstack = bootloader_init_alloc(&relocate, STACKSZ, &kstacksz);
  init->kstacksz = kstacksz;

  init->alloc = bootloader_init_alloc(&relocate, ALLOCSZ, &allocsz);
  init->allocsz = allocsz;

  return init;
}
