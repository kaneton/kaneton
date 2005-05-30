/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * paging.c
 * 
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun May 29 00:38:50 2005   mycure
 * last update   Mon May 30 22:26:56 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

extern t_kaneton	kaneton;

t_pde*			pd __ALIGNED__(4096) = (t_pde*)PAGING_PD;
t_pte*			pt0 __ALIGNED__(4096) = (t_pte*)PAGING_PT0;
t_pte*			pt1 __ALIGNED__(4096) = (t_pte*)PAGING_PT1;

/*
 * this function dumps a page table in a human readable form which
 * is very useful for debugging.
 */

void			paging_dump_table(t_pte*	table,
					  t_opts	opts)
{
  t_uint16		i;

  cons_msg('#', "dumping table 0x%x entries\n", table);
  printf("\n");
  printf("     |  no  |  address   |            flags            |\n");
  printf("     ---------------------------------------------------\n");

  for (i = 0; i < PAGING_NPTE; i++)
    {
      char		hrf[13];

      if ((opts & OPTS_PRESENT) && !(table[i] & PAGING_P))
	continue;

      /*
       * human readable flags
       */

      memset(hrf, '.', 12);
      hrf[12] = 0;

      if ((table[i] & PAGING_GLOBAL) == PAGING_GLOBAL)
	hrf[3] = 'G';

      if ((table[i] & PAGING_PS) == PAGING_PS)
	hrf[4] = '4';

      if ((table[i] & PAGING_DIRTY) == PAGING_DIRTY)
	hrf[5] = 'D';

      if ((table[i] & PAGING_ACCESSED) == PAGING_ACCESSED)
	hrf[6] = 'A';

      if ((table[i] & PAGING_PCD) == PAGING_PCD)
	hrf[7] = 'C';

      if ((table[i] & PAGING_PWT) == PAGING_PWT)
	hrf[8] = 'T';

      if ((table[i] & PAGING_U) == PAGING_U)
	hrf[9] = 'U';
      else if ((table[i] & PAGING_S) == PAGING_S)
	hrf[9] = 'S';

      if ((table[i] & PAGING_RW) == PAGING_RW)
	hrf[10] = 'W';
      else if ((table[i] & PAGING_RO) == PAGING_RO)
	hrf[10] = 'R';

      if ((table[i] & PAGING_P) == PAGING_P)
	hrf[11] = 'P';

      printf("     | %04u | 0x%08x | %s (%012b) |\n",
	     i,
	     (table[i] & PAGING_ADDRESS_MASK),
	     hrf, (table[i] & PAGING_FLAGS_MASK));
    }

  printf("\n");
}

/*
 * this function dumps a page directory
 */

void			paging_dump_directory(t_pde*	directory,
					      t_opts	opts)
{
  paging_dump_table((t_pte*)directory, opts);
}

/*
 * this function enables the paging mode setting one bit in cr0.
 */

void			paging_enable(void)
{
  asm volatile ("movl %%cr0, %%eax\n"
		"orl $0x80000000, %%eax\n"
		"movl %%eax, %%cr0\n"
		::);
}

/*
 * this function initializes the paging.
 *
 * this function:
 *
 * 1) installs the page directory
 * 2) installs the identity mapping via the first page table
 * 3) installs extra identity mapping with the second page table
 *    to be able to map the kernel stack
 * 4) load the new page directory
 * 5) enable the paging mode
 */

void			paging_init(void)
{
  t_uint32		addr;
  t_uint32		i;

  /*
   * 1)
   */

  memset(pd, 0x0, PAGING_NPDE * sizeof(t_pde));

  pd[0] = (t_uint32)pt0 | PAGING_P | PAGING_RW | PAGING_S;
  pd[1] = (t_uint32)pt1 | PAGING_P | PAGING_RW | PAGING_S;

  /*
   * 2)
   */

  memset(pt0, 0x0, PAGING_NPTE * sizeof(t_pte));

  for (i = 0, addr = 0; i < PAGING_NPTE; i++, addr += 4096)
    pt0[PAGING_PTE(addr)] = addr | PAGING_P | PAGING_RW | PAGING_S;

  /*
   * 3)
   */

  memset(pt1, 0x0, PAGING_NPTE * sizeof(t_pte));

  for (i = 0;
       addr < (KANETON_KERNEL_STACK + KANETON_KERNEL_STACKSZ);
       i++, addr += 4096)
    pt1[PAGING_PTE(addr)] = addr | PAGING_P | PAGING_RW | PAGING_S;

  /*
   * 4)
   */

  LCR3(pd);

  /*
   * 5)
   */

  paging_enable();

  cons_msg('+', "paging enabled\n");
}

/*

t_pde*pgd = (t_pde *)PMAP_PD_ADDR;
t_pte*ptk = (t_pte *)PMAP_PTK_ADDR;
t_pte*ptm = (t_pte *)PMAP_PTM_ADDR;
t_pte*pts = (t_pte *)PMAP_PTS_ADDR;
t_pte*pth = (t_pte *)PMAP_PTH_ADDR;
t_pte*ptt = (t_pte *)PMAP_PTT_ADDR;
t_pte*pt0 = (t_pte *)PMAP_PT0_ADDR;

voidvmap_init(multiboot_info_t *minfo)
{
  module_t*mod = (module_t *)minfo->mods_addr;
  intindex;
  intpde;
  intpa;
  intva;
  inti;

  memset(pgd, 0, sizeof (t_pde) * PD_ENTRIES);
  memset(ptk, 0, sizeof (t_pte) * PT_ENTRIES);
  memset(ptm, 0, sizeof (t_pte) * PT_ENTRIES);
  memset(ptt, 0, sizeof (t_pte) * PT_ENTRIES);
  memset(pt0, 0, sizeof (t_pte) * PT_ENTRIES);
  memset(pts, 0, sizeof (t_pte) * PT_ENTRIES);
  memset(pth, 0, sizeof (t_pte) * PT_ENTRIES);

  // identity mapping with pt0
  // liveness mapping

  pde = (0x0 & PD_MASK) >> PD_SHIFT;
  pgd[pde] =  PMAP_PT0_ADDR | PG_RW | PG_P;

  for (index = 0; index < 1024; index++)
    {
      pa = index * PAGESZ;
      va = index * PAGESZ;
      pde = (va & PT_MASK) >> PT_SHIFT;
      pt0[pde] = pa | PG_RW | PG_P;
    }

  // kernel mapping

  pde = (VMAP_KERN_ADDR & PD_MASK) >> PD_SHIFT;
  pgd[pde] = PMAP_PTK_ADDR | PG_RW | PG_P;

  for (i = 0, index = mod->mod_start;
       index < AROUND(mod->mod_end);
       index += PAGESZ, i++)
    {
      pa = index;
      va = VMAP_KERN_ADDR + (i * PAGESZ);
      pde = (va & PT_MASK) >> PT_SHIFT;
      ptk[pde] = pa | PG_RW | PG_P;
    }

  // console mapping

  pa = PMAP_CONS_ADDR;
  va = VMAP_CONS_ADDR;
  pde = (va & PT_MASK) >> PT_SHIFT;
  ptk[pde] = pa | PG_RW | PG_P;

  // memory management mapping

  pde = (VMAP_PTT_ADDR & PD_MASK) >> PD_SHIFT;
  pgd[pde] = PMAP_PTM_ADDR | PG_RW | PG_P;

  pa = PMAP_PTT_ADDR;
  va = VMAP_PTT_ADDR;
  pde = (va & PT_MASK) >> PT_SHIFT;
  ptm[pde] = pa | PG_RW | PG_P;

  pa = PMAP_MNG_ADDR;
  va = VMAP_MNG_ADDR;
  pde = (va & PT_MASK) >> PT_SHIFT;
  ptm[pde] = pa | PG_RW | PG_P;

  // super page table mapping

  pde = (VMAP_TPAGE_ADDR & PD_MASK) >> PD_SHIFT;
  pgd[pde] = PMAP_PTT_ADDR | PG_RW | PG_P;

  for (index = 0; index < PMAP_KERN_NPGS; index++)
    {
      pa = PMAP_PD_ADDR + (index * PAGESZ);
      va = VMAP_TPAGE_ADDR + (index * PAGESZ);
      pde = (va & PT_MASK) >> PT_SHIFT;
      ptt[pde] = pa | PG_RW | PG_P;
    }

  // heap mapping

  pde = (VMAP_HEAP_ADDR & PD_MASK) >> PD_SHIFT;
  pgd[pde] = PMAP_PTH_ADDR | PG_RW | PG_P;

  // stack mapping

  pde = (VMAP_STACK_ADDR & PD_MASK) >> PD_SHIFT;
  pgd[pde] = PMAP_PTS_ADDR | PG_RW | PG_P;

  pa = PMAP_STACK_ADDR;
  va = VMAP_STACK_ADDR;
  pde = (va & PT_MASK) >> PT_SHIFT;
  pts[pde] = pa | PG_RW | PG_P;

  pg_enable();
}


*/
