/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-smp/paging.c
 *
 * created       julien quintard   [sun may 29 00:38:50 2005]
 * updated       matthieu bucchianeri   [tue mar 13 11:46:11 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
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

t_ia32_directory	pd;
t_ia32_table		pt0;
t_ia32_table		pt;

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
 * 4) map the APIC registers memory.
 * 5) installs extra identity mapping to be able to map the kernel code,
 *    the kernel stack, the global offset table, the modules etc.
 * 6) loads the new page directory.
 * 7) enables the paging mode.
 */

void			bootloader_paging_init(void)
{
  t_uint32		limit;
  t_uint32		addr;
  t_uint16		i;
  t_ia32_page		pg;

  /*
   * 1)
   */

  if (pd_build(bootloader_init_alloc(PD_MAX_ENTRIES * sizeof(t_ia32_pde),
				     NULL), &pd, 1) != ERROR_NONE)
    {
      printf("cannot build a page-directory\n");
      bootloader_error();
    }

  /*
   * 2)
   */

  init->machdep.pd = pd;

  /*
   * 3)
   */

  if (pt_build(bootloader_init_alloc(PT_MAX_ENTRIES * sizeof(t_ia32_pte),
				     NULL), &pt0, 1) != ERROR_NONE)
    {
      printf("cannot build a page-table\n");
      bootloader_error();
    }

  pt0.present = 1;
  pt0.rw = PT_WRITABLE;
  pt0.user = PT_PRIVILEGED;
  pt0.cached = PT_CACHED;
  pt0.writeback = PT_WRITETHROUGH;

  pd_add_table(&pd, 0, pt0);

  pg.present = 1;
  pg.rw = PG_WRITABLE;
  pg.user = PG_PRIVILEGED;
  pg.cached = PT_CACHED;
  pg.writeback = PT_WRITETHROUGH;
  for (i = 0, addr = PAGESZ;  i < PT_MAX_ENTRIES - 1; i++, addr += PAGESZ)
    {
      pg.addr = addr;
      pt_add_page(&pt0, PTE_ENTRY(addr), pg);
    }

  /*
   * 4)
   */

  if (pt_build(bootloader_init_alloc(PT_MAX_ENTRIES * sizeof(t_ia32_pte),
				     NULL), &pt, 1) != ERROR_NONE)
    {
      printf("cannot build a page-table\n");
      bootloader_error();
    }

  pt.present = 1;
  pt.rw = PT_WRITABLE;
  pt.user = PT_PRIVILEGED;
  pt.cached = PT_CACHED;
  pt.writeback = PT_WRITETHROUGH;

  pd_add_table(&pd, PDE_ENTRY(APIC_REG_BASE), pt);

  pg.addr = APIC_REG_BASE;
  pt_add_page(&pt, PTE_ENTRY(APIC_REG_BASE), pg);

  /*
   * 5)
   */

  limit = bootloader_init_alloc(0, NULL);

  for (addr = init->kcode; addr < limit; addr += PAGESZ)
    {
      if (pd_get_table(&pd, PDE_ENTRY(addr), &pt) != ERROR_NONE)
	{
	  if (pt_build(bootloader_init_alloc(PT_MAX_ENTRIES *
					     sizeof(t_ia32_pte),
					     NULL), &pt, 1) != ERROR_NONE)
	    {
	      printf("cannot build a page-table\n");
	      bootloader_error();
	    }

	  pt.present = 1;
	  pt.rw = PT_WRITABLE;
	  pt.user = PT_PRIVILEGED;
	  pt.cached = PT_CACHED;
	  pt.writeback = PT_WRITETHROUGH;

	  pd_add_table(&pd, PDE_ENTRY(addr), pt);
	  limit += PAGESZ;
	}
      pg.addr = addr;
      pt_add_page(&pt, PTE_ENTRY(addr), pg);
    }

  /*
   * 6)
   */

  pd_activate(pd, PD_CACHED, PD_WRITETHROUGH);

  /*
   * 7)
   */

  paging_enable();
}

/*
 * this function init paging for an AP. since we need the kernel to be
 * runnable on this processor, we  setup the current address space (as
 * created above).
 *
 * steps:
 *
 * 1) setup the PDBR.
 * 2) activate paging on this processor.
 */

void			bootloader_paging_ap_init(void)
{
  /*
   * 1)
   */

  pd_activate(init->machdep.pd, PD_CACHED, PD_WRITETHROUGH);

  /*
   * 2)
   */

  paging_enable();
}

/*                                                                 [cut] /k1 */
