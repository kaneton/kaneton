/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-virtual/paging.c
 *
 * created       julien quintard   [sun may 29 00:38:50 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:16:53 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*			init;

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

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function initializes the paging.
 *
 * steps:
 *
 * 1) allocate and initialize the page directory.
 * 2) set the page directory address into the init variable.
 * 3) install the identity mapping via the first page table.
 * 4) map the APIC registers memory.
 * 5) install extra identity mapping to be able to map the kernel code,
 *    the kernel stack, the global offset table, the modules etc.
 * 6) load the new page directory.
 * 7) enable the paging mode.
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

  if (pd_build(bootloader_init_alloc(IA32_PD_MAX_ENTRIES * sizeof(t_ia32_pde),
				     NULL), &pd, 1) != STATUS_OK)
    {
      printf("cannot build a page-directory\n");
      bootloader_error();
    }

  /*
   * 2)
   */

  init->machine.pd = pd;

  /*
   * 3)
   */

  if (pt_build(bootloader_init_alloc(IA32_PT_MAX_ENTRIES * sizeof(t_ia32_pte),
				     NULL), &pt0, 1) != STATUS_OK)
    {
      printf("cannot build a page-table\n");
      bootloader_error();
    }

  pt0.present = 1;
  pt0.rw = IA32_PT_WRITABLE;
  pt0.user = IA32_PT_PRIVILEGED;
  pt0.cached = IA32_PT_CACHED;
  pt0.writeback = IA32_PT_WRITEBACK;

  pd_add_table(&pd, 0, pt0);

  pg.present = 1;
  pg.rw = IA32_PG_WRITABLE;
  pg.user = IA32_PG_PRIVILEGED;
  pg.cached = IA32_PG_CACHED;
  pg.writeback = IA32_PG_WRITEBACK;

  for (i = 0, addr = PAGESZ;  i < IA32_PT_MAX_ENTRIES - 1; i++, addr += PAGESZ)
    {
      pg.addr = addr;
      pt_add_page(&pt0, IA32_PTE_ENTRY(addr), pg);
    }

  /*
   * 4)
   */

  if (pt_build(bootloader_init_alloc(IA32_PT_MAX_ENTRIES * sizeof(t_ia32_pte),
				     NULL), &pt, 1) != STATUS_OK)
    {
      printf("cannot build a page-table\n");
      bootloader_error();
    }

  pt.present = 1;
  pt.rw = IA32_PT_WRITABLE;
  pt.user = IA32_PT_PRIVILEGED;
  pt.cached = IA32_PT_CACHED;
  pt.writeback = IA32_PT_WRITETHROUGH;

  pd_add_table(&pd, IA32_PDE_ENTRY(APIC_REG_BASE), pt);

  pg.addr = APIC_REG_BASE;
  pt_add_page(&pt, IA32_PTE_ENTRY(APIC_REG_BASE), pg);

  /*
   * 5)
   */

  limit = bootloader_init_alloc(0, NULL);

  for (addr = init->kcode; addr < limit; addr += PAGESZ)
    {
      if (pd_get_table(&pd, IA32_PDE_ENTRY(addr), &pt) != STATUS_OK)
	{
	  if (pt_build(bootloader_init_alloc(IA32_PT_MAX_ENTRIES *
						  sizeof(t_ia32_pte),
						  NULL), &pt, 1) != STATUS_OK)
	    {
	      printf("cannot build a page-table\n");
	      bootloader_error();
	    }

	  pt.present = 1;
	  pt.rw = IA32_PT_WRITABLE;
	  pt.user = IA32_PT_PRIVILEGED;
	  pt.cached = IA32_PT_CACHED;
	  pt.writeback = IA32_PT_WRITEBACK;

	  pd_add_table(&pd, IA32_PDE_ENTRY(addr), pt);
	  limit += PAGESZ;
	}
      pg.addr = addr;
      pt_add_page(&pt, IA32_PTE_ENTRY(addr), pg);
    }

  /*
   * 6)
   */

  pd_activate(pd, IA32_PD_CACHED, IA32_PD_WRITEBACK);

  /*
   * 7)
   */

  asm volatile("movl %%cr0, %%eax\n\t"
	       "orl $0x80000000, %%eax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax", "memory");
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

  pd_activate(init->machine.pd, IA32_PD_CACHED, IA32_PD_WRITETHROUGH);

  /*
   * 2)
   */

  asm volatile("movl %%cr0, %%eax\n\t"
	       "orl $0x80000000, %%eax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax", "memory");
}
