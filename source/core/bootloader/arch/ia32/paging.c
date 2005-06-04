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
 * last update   Wed Jun  1 12:53:08 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * the memory description variable
 */

extern t_memory		memory;

/*
 * the kernel page directory and page tables
 */

t_pde*			pd __ALIGNED__(4096) = (t_pde*)PAGING_PD;
t_pte*			pt0 __ALIGNED__(4096) = (t_pte*)PAGING_PT0;
t_pte*			pt1 __ALIGNED__(4096) = (t_pte*)PAGING_PT1;

/*
 * this function dumps a page table in a human readable form which
 * is very useful for debugging.
 */

#if (IA32_DEBUG & IA32_DEBUG_PAGING)
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
#endif

/*
 * this function dumps a page directory
 */

#if (IA32_DEBUG & IA32_DEBUG_PAGING)
void			paging_dump_directory(t_pde*	directory,
					      t_opts	opts)
{
  paging_dump_table((t_pte*)directory, opts);
}
#endif

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
 * 3) installs extra identity mapping to be able to map the kernel code,
 *    the kernel stack and the GDT
 * 4) loads the new page directory
 * 5) enables the paging mode
 * 6) updates the memory description variable
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
       addr < (BOOTLOADER_KSTACK + BOOTLOADER_KSTACKSZ);
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

  /*
   * 6)
   */

  memory.areas[2].address = PAGING_PT0;
  memory.areas[2].size = 3 * PAGESZ; /* PD, PT0, PT1 */

XXX

  /* XXX faire en sorte de copier le necessaire des infos pour que
   *     ca fonctionne une fois le kernel lance.
   *   on doit copier: t_memory, t_area[], t_module[]
   * genre calculer pour faire ca:
   *
   * [t_memory: areas, modules][areas][modules]
   *             |_______|_______^        ^
   *                     |________________|
   */
}
