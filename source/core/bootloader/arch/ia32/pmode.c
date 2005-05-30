/*
 * kaneton
 *
 * gdt.c
 *
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Mon Jul 19 20:43:14 2004   mycure
 * last update   Mon May 30 15:20:56 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

t_gdte*			gdt __ALIGNED__(8) = (t_gdte*)GDT_ADDR;
t_gdtr			gdtr;

/*
 * this function dumps the global offset tables in a human readable form
 * which is very very useful for debugging.
 */

#if (IA32_DEBUG & IA32_DEBUG_GDT)
void			gdt_dump(void)
{
  t_uint16		entries = gdtr.size / sizeof(t_gdte);
  t_uint16		i;

  cons_msg('#', "dumping global offset table 0x%x entries\n", gdt);
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

      if ((gdt[i].type & GDT_PRESENT) == GDT_PRESENT)
	hrt[0] = 'P';

      if ((gdt[i].type & GDT_DPL3) == GDT_DPL3)
	hrt[1] = '3';
      else if ((gdt[i].type & GDT_DPL2) == GDT_DPL2)
	hrt[1] = '2';
      else if ((gdt[i].type & GDT_DPL1) == GDT_DPL1)
	hrt[1] = '1';
      else if ((gdt[i].type & GDT_DPL0) == GDT_DPL0)
	hrt[1] = '0';

      if ((gdt[i].type & GDT_S) == GDT_S)
	hrt[2] = 'S';

      if ((gdt[i].type & GDT_CODE) == GDT_CODE)
	hrt[3] = 'C';
      else if ((gdt[i].type & GDT_DATA) == GDT_DATA)
	hrt[3] = 'D';

      /*
       * human readable flags
       */

      memset(hrf, '.', 3);
      hrf[3] = 0;

      if ((gdt[i].flags & GDT_GRANULAR) == GDT_GRANULAR)
	hrf[0] = 'G';

      if ((gdt[i].flags & GDT_USE32) == GDT_USE32)
	{
	  hrf[1] = '3';
	  hrf[2] = '2';
	}
      else if ((gdt[i].flags & GDT_USE16) == GDT_USE16)
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
 * this function update the segment registers to work with the new
 * global offset table.
 */

void			gdt_update_registers(t_uint16	gdt_kernel_cs,
					     t_uint16	gdt_kernel_ds)
{
  t_reg16		cs = (gdt_kernel_cs << 3) | GDT_TI_GDT | 0;
  t_reg16		ds = (gdt_kernel_ds << 3) | GDT_TI_GDT | 0;

  asm volatile ("pushl %0\n"
		"pushl $gdt_update_registers_label\n"
		"lret\n"
		"gdt_update_registers_label:\n"
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
 * this function installs the protected mode setting one bit in cr0.
 */

void			gdt_enable(void)
{
  asm volatile ("movl %%cr0, %%eax\n"
		"orw %%ax, 1\n"
		"movl %%eax, %%cr0\n"
		::);
}

/*
 * this function sets a new entry in the current global offset table.
 */

void			gdt_set(t_uint16		entry,
				t_paddr			base,
				t_psize			limit,
				t_uint8			type,
				t_uint8			flags)
{
  if (entry >= GDT_ENTRIES)
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
 * this function initializes the global offset tables inserting
 * height entries for the kernel, drivers, services and user tasks.
 *
 * each segment has the same size with different rights: read/execution,
 * read/write etc..
 *
 * then the function loads the new global offset table, updates the
 * segment registers and finally installs the protected mode.
 */

void			gdt_init(void)
{
  memset(gdt, 0x0, GDT_ENTRIES * sizeof(t_gdte));

  gdt_set(GDT_KERNEL_CS, 0x0, 0xffffffff,
	  GDT_PRESENT | GDT_DPL0 | GDT_S | GDT_CODE,
	  GDT_GRANULAR | GDT_USE32);

  gdt_set(GDT_KERNEL_DS, 0x0, 0xffffffff,
	  GDT_PRESENT | GDT_DPL0 | GDT_S | GDT_DATA,
	  GDT_GRANULAR | GDT_USE32);

  gdt_set(GDT_DRIVER_CS, 0x0, 0xffffffff,
	  GDT_PRESENT | GDT_DPL1 | GDT_S | GDT_CODE,
	  GDT_GRANULAR | GDT_USE32);

  gdt_set(GDT_DRIVER_DS, 0x0, 0xffffffff,
	  GDT_PRESENT | GDT_DPL1 | GDT_S | GDT_DATA,
	  GDT_GRANULAR | GDT_USE32);

  gdt_set(GDT_SERVICE_CS, 0x0, 0xffffffff,
	  GDT_PRESENT | GDT_DPL2 | GDT_S | GDT_CODE,
	  GDT_GRANULAR | GDT_USE32);

  gdt_set(GDT_SERVICE_DS, 0x0, 0xffffffff,
	  GDT_PRESENT | GDT_DPL2 | GDT_S | GDT_DATA,
	  GDT_GRANULAR | GDT_USE32);

  gdt_set(GDT_USER_CS, 0x0, 0xffffffff,
	  GDT_PRESENT | GDT_DPL3 | GDT_S | GDT_CODE,
	  GDT_GRANULAR | GDT_USE32);

  gdt_set(GDT_USER_DS, 0x0, 0xffffffff,
	  GDT_PRESENT | GDT_DPL3 | GDT_S | GDT_DATA,
	  GDT_GRANULAR | GDT_USE32);

  gdtr.address = (t_uint32)gdt;
  gdtr.size = (t_uint16)(GDT_VALID_ENTRIES * sizeof(t_gdte));

  LGDT(gdtr);

  gdt_update_registers(GDT_KERNEL_CS, GDT_KERNEL_DS);

  gdt_enable();

#if (IA32_DEBUG & IA32_DEBUG_GDT)
  gdt_dump();
#endif

}

/*
void gdt_reinit()
{
  gdtr.size = (u_int16_t) 5 * sizeof (t_gdte);
  gdtr.addr = (u_int32_t) VMAP_GDT_ADDR;

  gdt_install();
  gdt_cpu(KERNEL_CS, KERNEL_DS);
}
*/
