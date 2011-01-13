/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...e/architecture/ia32/educational/paging.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:05 2005]
 * updated       julien quintard   [thu jan 13 12:50:55 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file provides basic paging functionalities.
 *
 * note that the ia32/educational implementation makes use of the mirroring
 * technique. this technique is used to prevent the kernel from looping
 * infinitely whenever trying to map a page table for instance. indeed,
 * whenever a page table must be mapped, a virtual memory area is picked.
 * this memory area must be mapped by creating a mapping in the page
 * directory/table hierarchical structure. in order to do so, the kernel page
 * directory and the kernel page table referencing the given area must
 * also be mapped. should not this be the case, the referencing page table,
 * for example, should be mapped by creating a mapping etc. there is an
 * obvious infinite loop occuring here where every page table which has
 * to be mapped implies another page table to reference it, hence to be
 * mapped as well.
 *
 * in order to prevent this infinite loop, the kernel relies on the
 * mirroring technique. this mechanism consists in setting up a kernel
 * page directory entry as acting as a special loopback. for example, the
 * last page directory entry, i.e pd[1023], will not reference another page
 * table as it is the case for the other page directory entries but the
 * page directory itself. this way, the page directory, whenever the
 * mirroring page directory entry is used, is considered by the microprocessor
 * as page table and its entries as page tables entries.
 *
 * the following schema depicts this organization:
 *
 * pd @ 0x011e2000
 *   pde 0 references 0x011e3000
 *     pt @ 0x011e3000
 *       pte 0 not present
 *       pte 1 references 0x00001000
 *       pte 2 references 0x00002000
 *       pte 3 references 0x00003000
 *       pte 4 references 0x00004000
 *       ...
 *   pde 2 not present
 *   pde 3 not present
 *   pde 4 references 0x011e4000
 *     pt @ 0x011e4000
 *       pte 0 references 0x01000000
 *       pte 1 references 0x01001000
 *       pte 2 references 0x01002000
 *   ...
 *   pde 1023 references 0x011e2000 (the page directory itself)
 *     pt @ 0x011e2000 (the page directory acts as a page table)
 *       pte 0 references 0x011e3000 (the first page table: pde 0)
 *       pte 4 references 0x011e3000 (the second page table: pde1)
 *       pte 1023 references 0x011e3000 (the page directory: pde 1023)
 *
 * this technique---assuming the kernel page directory is mapped, but this is
 * quite obvious---enables the kernel to modify any page directory/table
 * without mapping any intermediate page table. indeed, the kernel only needs
 * to reference the page table it wishes to modify through its mirrored
 * page directory entry.
 *
 * this technique implies that the last page directory entry is reserved
 * for the mirroring mechanism. therefore, the last 4MB of virtual memory
 * must be locked for that purpose meaning that 4MB of memory are wasted.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the init structure.
 */

extern s_init*		_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the system's current page directory.
 */

at_pd			_architecture_pd;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function enables the paging mechanism by setting the higher
 * bit of CR0. besides, the global pages support is activated by setting
 * the bit 7 of CR4.
 */

t_error			architecture_paging_enable(void)
{
  asm volatile("movl %%cr0, %%eax\n\t"
	       "orl $0x80000000, %%eax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax", "memory");

  asm volatile("movl %%cr4, %%eax\n\t"
	       "orl $128, %%eax\n\t"
	       "movl %%eax, %%cr4\n\t"
	       :
	       :
	       : "%eax");

  MACHINE_LEAVE();
}

/*
 * this function sets up the paging by importing the boot loader's page
 * directory.
 *
 * steps:
 *
 * 1) import the boot loader's page directory.
 * 2) enable the paging mechanism.
 */

t_error			architecture_paging_setup(void)
{

  /*
   * 1)
   */

  _architecture_pd = (at_pd)_init->machine.pd;

  /*
   * 2)
   */

  if (architecture_paging_enable() != ERROR_OK)
    MACHINE_ESCAPE("unable to enable the paging mechanism");

  MACHINE_LEAVE();
}

/*
 * this function generates the CR3 register's content so that to be
 * overwritten, hence referencing another page directory structure.
 *
 * steps:
 *
 * XXX
 */

t_error			architecture_paging_cr3(at_pd		pd,
						t_flags		flags,
						at_cr3*		cr3)
{
  /*
   * 0)
   */

  if (cr3 == NULL)
    MACHINE_ESCAPE("the 'cr3' argument is null");

  /*
   * 1)
   */

  *cr3 = ((t_vaddr)pd & 0xfffff000) | flags;

  MACHINE_LEAVE();
}

/*
 * this function imports the given page directory, making it the
 * current one.
 *
 * steps:
 *
 * XXX
 */

t_error			architecture_paging_import(at_pd	pd,
						   t_flags	flags)
{
  at_cr3		cr3;

  /*
   * 1)
   */

  cr3 = ((t_vaddr)pd & 0xfffff000) | flags;

  /*
   * 2)
   */

  ARCHITECTURE_LCR3(cr3);

  /*
   * 3)
   */

  _architecture_pd = pd;

  MACHINE_LEAVE();
}
