/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/bootloader/arch/ia32-virtual/paging.c
 *
 * created       julien quintard   [sun may 29 00:38:50 2005]
 * updated       matthieu bucchianeri   [thu jan  5 11:54:00 2006]
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * these function just install the paging mode to enable virtual memory
 * which is required by the kaneton kernel.
 *
 * think about mapping everything needed: kernel code, kernel stack, init
 * structure etc..
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*			init;

/*                                                                  [cut] k1 */

/*
 * the kernel page directory and page tables.
 *
 * the page table pt0 is used for identity mapping from 0 to 4 Mb while
 * the page table pt is dynamically used for identity mapping from
 * 16 Mb to ... approximately 20 Mb.
 */

t_directory		pd;
t_table			pt0;
t_table			pt;

/*                                                                 [cut] /k1 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k1 */

/*
 * this function initialises the paging.
 *
 * steps:
 *
 * 1) allocates and initialises the page directory.
 * 2) sets the page directory address into the init variable.
 * 3) installs the identity mapping via the first page table.
 * 4) installs extra identity mapping to be able to map the kernel code,
 *    the kernel stack, the global offset table, the modules etc..
 * 5) loads the new page directory.
 * 6) enables the paging mode.
 */

void			bootloader_paging_init(void)
{
  t_uint32		limit;
  t_uint32		addr;
  t_uint16		i;
  t_page		pg;

  /*
   * 1)
   */

  pd_build(bootloader_init_alloc(PD_MAX_ENTRIES * sizeof(t_pde), NULL),
	   &pd, 1);

  /*
   * 2)
   */

  init->machdep.pd = pd;

  /*
   * 3)
   */

  pt_build(bootloader_init_alloc(PT_MAX_ENTRIES * sizeof(t_pte), NULL),
	   &pt0, 1);

  pt0.present = 1;
  pt0.rw = 1;
  pt0.user = 0;

  pd_add_table(&pd, 0, pt0);

  pg.present = 1;
  pg.rw = 1;
  pg.user = 0;
  for (i = 0, addr = 0;  i < PT_MAX_ENTRIES; i++, addr += PAGE_SIZE)
    {
      pg.addr = (void*)addr;
      pt_add_page(&pt0, PTE_ENTRY(addr), pg);
    }

  /*
   * 4)
   */

  limit = bootloader_init_alloc(0, NULL);

  for (addr = INIT_RELOCATE; addr < limit; addr += PAGE_SIZE)
    {
      if (pd_get_table(&pd, PDE_ENTRY(addr), &pt) != ERROR_NONE)
	{
	  pt_build(bootloader_init_alloc(PT_MAX_ENTRIES * sizeof(t_pte), NULL),
		   &pt, 1);

	  pt.present = 1;
	  pt.rw = 1;
	  pt.user = 0;

	  pd_add_table(&pd, PDE_ENTRY(addr), pt);
	}

      pg.addr = (void*)addr;
      pt_add_page(&pt, PTE_ENTRY(addr), pg);
    }

  /*
   * 5)
   */

  pd_activate(pd);

  /*
   * 6)
   */

  paging_enable();
}

/*                                                                 [cut] /k1 */
