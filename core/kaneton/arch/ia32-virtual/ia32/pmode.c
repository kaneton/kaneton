/*
 * kaneton
 *
 * pmode.c
 *
 * path          /home/mycure/kaneton
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Mon Jul 19 20:43:14 2004   mycure
 * last update   Tue Nov 15 20:13:49 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains everything needed to perform operations on the
 * global offset table.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * the system's global offset table.
 */

t_gdte*			gdt;
t_gdtr			gdtr;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * this function dumps the global offset table in a human readable form
 * which is very very useful for debugging.
 */

#if (IA32_DEBUG & IA32_DEBUG_PMODE)
t_error			pmode_gdt_dump(void)
{
  t_uint16		entries = gdtr.size / sizeof(t_gdte);
  t_uint16		i;

  cons_msg('#', "dumping global offset table 0x%x entries\n\n", gdt);

  cons_msg('#', "   |  no  |       base - limit      |      type       |"
	   " flags    |\n");
  cons_msg('#', "   ----------------------------------------------------"
	   "-------------\n");

  for (i = 0; i < entries; i++)
    {
      char		hrt[5];
      char		hrf[4];

      /*
       * human readable type
       */

      memset(hrt, '.', 4);
      hrt[4] = 0;

      if ((gdt[i].type & PMODE_GDT_PRESENT) == PMODE_GDT_PRESENT)
	hrt[0] = 'P';

      if ((gdt[i].type & PMODE_GDT_DPL3) == PMODE_GDT_DPL3)
	hrt[1] = '3';
      else if ((gdt[i].type & PMODE_GDT_DPL2) == PMODE_GDT_DPL2)
	hrt[1] = '2';
      else if ((gdt[i].type & PMODE_GDT_DPL1) == PMODE_GDT_DPL1)
	hrt[1] = '1';
      else if ((gdt[i].type & PMODE_GDT_DPL0) == PMODE_GDT_DPL0)
	hrt[1] = '0';

      if ((gdt[i].type & PMODE_GDT_S) == PMODE_GDT_S)
	hrt[2] = 'S';

      if ((gdt[i].type & PMODE_GDT_CODE) == PMODE_GDT_CODE)
	hrt[3] = 'C';
      else if ((gdt[i].type & PMODE_GDT_DATA) == PMODE_GDT_DATA)
	hrt[3] = 'D';

      /*
       * human readable flags
       */

      memset(hrf, '.', 3);
      hrf[3] = 0;

      if ((gdt[i].flags & PMODE_GDT_GRANULAR) == PMODE_GDT_GRANULAR)
	hrf[0] = 'G';

      if ((gdt[i].flags & PMODE_GDT_USE32) == PMODE_GDT_USE32)
	{
	  hrf[1] = '3';
	  hrf[2] = '2';
	}
      else if ((gdt[i].flags & PMODE_GDT_USE16) == PMODE_GDT_USE16)
	{
	  hrf[1] = '1';
	  hrf[2] = '6';
	}

      cons_msg('#', "   | %04u | 0x%08x - 0x%08x | %s (%08b) | %s (%04b) |\n",
	       i,
	       (gdt[i].base_00_15 & 0x0000ffff) |
	       ((gdt[i].base_16_23 << 16) & 0x00ff0000) |
	       ((gdt[i].base_24_31 << 24) & 0xff000000),
	       (gdt[i].limit_00_15 & 0x0000ffff) |
	       ((gdt[i].limit_16_19 << 16) & 0x000f0000),
	       hrt, (gdt[i].type & 0x000000ff),
	       hrf, (gdt[i].flags & 0x000000f));
    }

  return (ERROR_NONE);
}
#endif

/*
 * this function updates the segment registers to work with the new
 * global offset table.
 */

t_error			pmode_update_registers(t_uint16		kcs,
					       t_uint16		kds)
{
  t_reg16		cs = (kcs << 3) | PMODE_TI_GDT | 0;
  t_reg16		ds = (kds << 3) | PMODE_TI_GDT | 0;

  asm ("pushl %0\n\t"
       "pushl $pmode_update_registers_label\n\t"
       "lret\n\t"
       "pmode_update_registers_label:\n\t"
       "movl %1, %%eax\n\t"
       "movw %%ax, %%ds\n\t"
       "movw %%ax, %%ss\n\t"
       "movw %%ax, %%es\n\t"
       "movw %%ax, %%fs\n\t"
       "movw %%ax, %%gs\n\t"
       :
       : "g" (cs), "g" (ds)
       : "memory", "%eax"
       );

  return (ERROR_NONE);
}

/*
 * this function installs the protected mode setting the first bit of cr0.
 */

t_error			pmode_enable(void)
{
  asm ("movl %%cr0, %%eax\n\t"
       "orw %%ax, 1\n\t"
       "movl %%eax, %%cr0\n\t"
       :
       :
       : "%eax"
       );

  return (ERROR_NONE);
}

/*
 * this function sets a new entry in the current global offset table.
 */

t_error			pmode_gdt_set(t_uint16			entry,
				      t_paddr			base,
				      t_psize			limit,
				      t_uint8			type,
				      t_uint8			flags)
{
  if (entry >= PMODE_GDT_ENTRIES)
    return (ERROR_UNKNOWN);

  gdt[entry].limit_00_15 = limit & 0x0000ffff;
  gdt[entry].base_00_15 = (t_uint32)base & 0x0000ffff;
  gdt[entry].base_16_23 = ((t_uint32)base >> 16) & 0x000000ff;
  gdt[entry].type = type;
  gdt[entry].limit_16_19 = (limit >> 16) & 0x0000000f;
  gdt[entry].flags = flags;
  gdt[entry].base_24_31 = ((t_uint32)base >> 24) & 0x0000000f;

  return (ERROR_NONE);
}

/*
 * this function initialises the global offset table.
 *
 * each segment has the same size with different rights: read/execution,
 * read/write etc..
 *
 * this function is called by the machine-dependent segment manager.
 *
 * steps:
 *
 * 1) reinitialises the pmode global variables gdt and gdtr.
 * 2) dumps the global offset table if required.
 */

t_error			pmode_init(void)
{
  /*
   * 1)
   */

  memcpy(&gdtr, &init->machdep.gdtr, sizeof(t_gdtr));

  gdt = (t_gdte*)gdtr.address;

  /*
   * 2)
   */

#if (IA32_DEBUG & IA32_DEBUG_PMODE)
  pmode_gdt_dump();
#endif

  return (ERROR_NONE);
}

/*
 * this function cleans the global offset table.
 */

t_error			pmode_clean(void)
{
  memset((void*)gdtr.address, 0x0, gdtr.size);

  return (ERROR_NONE);
}

/*                                                                 [cut] /k2 */
