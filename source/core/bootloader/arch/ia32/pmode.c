/*
 * kaneton
 *
 * pmode.c
 *
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Mon Jul 19 20:43:14 2004   mycure
 * last update   Sun Jun 19 16:15:08 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

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
 * this function dumps the global offset table in a human readable form
 * which is very very useful for debugging.
 */

#if (IA32_DEBUG & IA32_DEBUG_PMODE)
void			bootloader_pmode_gdt_dump(void)
{
  t_uint16		entries = gdtr.size / sizeof(t_gdte);
  t_uint16		i;

  bootloader_cons_msg('#', "dumping global offset table 0x%x entries\n", gdt);
  printf("\n");
  printf("     |  no  |       base - limit      |      type       |"
	 "   flags    |\n");
  printf("     ----------------------------------------------------"
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

      printf("     | %04u | 0x%08x - 0x%08x | %s (%08b) | %s (%04b) |\n",
	     i,
	     (gdt[i].base_00_15 & 0x0000ffff) |
	     ((gdt[i].base_16_23 << 16) & 0x00ff0000) |
	     ((gdt[i].base_24_31 << 24) & 0xff000000),
	     (gdt[i].limit_00_15 & 0x0000ffff) |
	     ((gdt[i].limit_16_19 << 16) & 0x000f0000),
	     hrt, (gdt[i].type & 0x000000ff),
	     hrf, (gdt[i].flags & 0x000000f));
    }

  printf("\n");
}
#endif

/*
 * this function updates the segment registers to work with the new
 * global offset table.
 */

void			bootloader_pmode_update_registers(t_uint16	kcs,
							  t_uint16	kds)
{
  t_reg16		cs = (kcs << 3) | PMODE_TI_GDT | 0;
  t_reg16		ds = (kds << 3) | PMODE_TI_GDT | 0;

  asm volatile ("pushl %0\n"
		"pushl $pmode_update_registers_label\n"
		"lret\n"
		"pmode_update_registers_label:\n"
		"movl %1, %%eax\n"
		"movw %%ax, %%ds\n"
		"movw %%ax, %%ss\n"
		"movw %%ax, %%es\n"
		"movw %%ax, %%fs\n"
		"movw %%ax, %%gs\n"
		:
		: "g" (cs), "g" (ds));
}

/*
 * this function installs the protected mode setting the first bit of cr0.
 */

void			bootloader_pmode_enable(void)
{
  asm volatile ("movl %%cr0, %%eax\n"
		"orw %%ax, 1\n"
		"movl %%eax, %%cr0\n"
		::);
}

/*
 * this function sets a new entry in the current global offset table.
 */

void			bootloader_pmode_gdt_set(t_uint16	entry,
						 t_paddr	base,
						 t_psize	limit,
						 t_uint8	type,
						 t_uint8	flags)
{
  if (entry >= PMODE_GDT_ENTRIES)
    bootloader_error();

  gdt[entry].limit_00_15 = limit & 0x0000ffff;
  gdt[entry].base_00_15 = (t_uint32)base & 0x0000ffff;
  gdt[entry].base_16_23 = ((t_uint32)base >> 16) & 0x000000ff;
  gdt[entry].type = type;
  gdt[entry].limit_16_19 = (limit >> 16) & 0x0000000f;
  gdt[entry].flags = flags;
  gdt[entry].base_24_31 = ((t_uint32)base >> 24) & 0x0000000f;
}

/*
 * this function initializes the global offset table inserting
 * height entries for the kernel, drivers, services and user tasks.
 *
 * each segment has the same size with different rights: read/execution,
 * read/write etc..
 *
 * steps:
 *
 * 1) allocates and initializes the memory for the global offset table.
 * 2) sets the global offset table address in the init variable.
 * 3) sets the height segments for the kernel, driver, service and user.
 * 4) loads the GDT.
 * 5) updates the segments registers.
 * 6) finally installs the protected mode.
 * 7) dumps the global offset table if required.
 */

void			bootloader_pmode_init(void)
{
  /*
   * 1)
   */

  gdt = (t_gdte*)bootloader_init_alloc(PMODE_GDT_ENTRIES * sizeof(t_gdte),
				       NULL);
  memset(gdt, 0x0, PMODE_GDT_ENTRIES * sizeof(t_gdte));

  /*
   * 2)
   */

  init->machdep.gdt = gdt;

  /*
   * 3)
   */

  bootloader_pmode_gdt_set(PMODE_GDT_KERNEL_CS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL0 | PMODE_GDT_S |
			   PMODE_GDT_CODE, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  bootloader_pmode_gdt_set(PMODE_GDT_KERNEL_DS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL0 | PMODE_GDT_S |
			   PMODE_GDT_DATA, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  bootloader_pmode_gdt_set(PMODE_GDT_DRIVER_CS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL1 | PMODE_GDT_S |
			   PMODE_GDT_CODE, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  bootloader_pmode_gdt_set(PMODE_GDT_DRIVER_DS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL1 | PMODE_GDT_S |
			   PMODE_GDT_DATA, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  bootloader_pmode_gdt_set(PMODE_GDT_SERVICE_CS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL2 | PMODE_GDT_S |
			   PMODE_GDT_CODE, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  bootloader_pmode_gdt_set(PMODE_GDT_SERVICE_DS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL2 | PMODE_GDT_S |
			   PMODE_GDT_DATA, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  bootloader_pmode_gdt_set(PMODE_GDT_USER_CS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL3 | PMODE_GDT_S |
			   PMODE_GDT_CODE, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  bootloader_pmode_gdt_set(PMODE_GDT_USER_DS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL3 | PMODE_GDT_S |
			   PMODE_GDT_DATA, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  /*
   * 4)
   */

  gdtr.address = (t_uint32)gdt;
  gdtr.size = (t_uint16)(PMODE_GDT_ENTRIES * sizeof(t_gdte));

  LGDT(gdtr);

  /*
   * 5)
   */

  bootloader_pmode_update_registers(PMODE_GDT_KERNEL_CS, PMODE_GDT_KERNEL_DS);

  /*
   * 6)
   */

  bootloader_pmode_enable();

  /*
   * 7)
   */

#if (IA32_DEBUG & IA32_DEBUG_PMODE)
  bootloader_pmode_gdt_dump();
#endif
}
