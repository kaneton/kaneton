/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * paging.c
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Sun May 29 00:38:50 2005   mycure
 * last update   Mon Jun 13 14:32:19 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

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

t_pde*			pd;
t_pte*			pt0;

t_pte*			pt;

/*
 * this function dumps a page table in a human readable form which
 * is very useful for debugging.
 */

#if (IA32_DEBUG & IA32_DEBUG_PAGING)
void			paging_dump_table(t_pte*		table,
					  t_opts		opts)
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
	     (table[i] & PAGING_ADDRESS),
	     hrf, (table[i] & PAGING_FLAGS));
    }

  printf("\n");
}
#endif

/*
 * this function dumps a page directory.
 */

#if (IA32_DEBUG & IA32_DEBUG_PAGING)
void			paging_dump_directory(t_pde*		directory,
					      t_opts		opts)
{
  paging_dump_table((t_pte*)directory, opts);
}
#endif

/*
 * this function enables the paging mode setting the last bit of cr0.
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
 * steps:
 *
 * 1) allocates and initializes the page directory.
 * 2) sets the page directory address into the init variable.
 * 3) installs the identity mapping via the first page table.
 * 4) installs extra identity mapping to be able to map the kernel code,
 *    the kernel stack, the global offset table, the modules etc..
 * 5) loads the new page directory.
 * 6) enables the paging mode.
 */

void			paging_init(void)
{
  t_uint32		limit;
  t_uint32		addr;
  t_uint16		i;

  /*
   * 1)
   */

  pd = (t_pde*)bootloader_alloc(PAGING_NPDE * sizeof(t_pde), NULL);
  memset(pd, 0x0, PAGING_NPDE * sizeof(t_pde));

  /*
   * 2)
   */

  init->machdep.pd = pd;

  /*
   * 3)
   */

  pt0 = (t_pte*)bootloader_alloc(PAGING_NPTE * sizeof(t_pte), NULL);
  memset(pt0, 0x0, PAGING_NPTE * sizeof(t_pte));

  pd[0] = (t_uint32)pt0 | PAGING_P | PAGING_RW | PAGING_S;

  for (i = 0, addr = 0; i < PAGING_NPTE; i++, addr += 4096)
    pt0[PAGING_PTE(addr)] = addr | PAGING_P | PAGING_RW | PAGING_S;

  /*
   * 4)
   */

  limit = bootloader_alloc(0, NULL);

  for (addr = INIT_RELOCATE; addr < limit; addr += 4096)
    {
      if ((pd[PAGING_PDE(addr)] & PAGING_ADDRESS) == 0)
	{
	  pt = (t_pte*)bootloader_alloc(PAGING_NPTE * sizeof(t_pte), NULL);
	  memset(pt, 0x0, PAGING_NPTE * sizeof(t_pte));

	  pd[PAGING_PDE(addr)] = (t_uint32)pt | PAGING_P |
	    PAGING_RW | PAGING_S;
	}
      else
	pt = (t_pte*)(pd[PAGING_PDE(addr)] & PAGING_ADDRESS);

      pt[PAGING_PTE(addr)] = addr | PAGING_P | PAGING_RW | PAGING_S;
    }

  /*
   * 5)
   */

  LCR3(pd);

  /*
   * 6)
   */

  paging_enable();

  cons_msg('+', "paging enabled\n");
}
