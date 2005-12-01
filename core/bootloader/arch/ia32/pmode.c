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
 * last update   Tue Nov 15 19:53:54 2005   mycure
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * the student just has to setup the protected mode in this file,
 * nothing more.
 *
 * of course, it will be better to provide some debug functions etc..
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

extern t_init*		init;

/*
 * the system's global offset table.
 */

t_gdte*			gdt;
t_gdtr			gdtr;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k1 */

/*
 * this function updates the segment registers to work with the new
 * global offset table.
 */

void			bootloader_pmode_update_registers(t_uint16	kcs,
							  t_uint16	kds)
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
}

/*
 * this function installs the protected mode setting the first bit of cr0.
 */

void			bootloader_pmode_enable(void)
{
  asm ("movl %%cr0, %%eax\n\t"
       "orw %%ax, 1\n\t"
       "movl %%eax, %%cr0\n\t"
       :
       :
       : "%eax"
       );
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
 * this function initialises the global offset table inserting
 * height entries for the core, drivers, services and programs.
 *
 * each segment has the same size with different rights: read/execution,
 * read/write etc..
 *
 * steps:
 *
 * 1) allocates and initialises the memory for the global offset table.
 * 2) sets the height segments for the core, driver, service and program.
 * 3) loads the GDT.
 * 4) updates the segments registers.
 * 5) finally installs the protected mode.
 * 6) sets the global offset table address in the init variable.
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

  bootloader_pmode_gdt_set(PMODE_GDT_CORE_CS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL0 | PMODE_GDT_S |
			   PMODE_GDT_CODE, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  bootloader_pmode_gdt_set(PMODE_GDT_CORE_DS, 0x0, 0xffffffff,
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

  bootloader_pmode_gdt_set(PMODE_GDT_PROGRAM_CS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL3 | PMODE_GDT_S |
			   PMODE_GDT_CODE, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  bootloader_pmode_gdt_set(PMODE_GDT_PROGRAM_DS, 0x0, 0xffffffff,
			   PMODE_GDT_PRESENT | PMODE_GDT_DPL3 | PMODE_GDT_S |
			   PMODE_GDT_DATA, PMODE_GDT_GRANULAR |
			   PMODE_GDT_USE32);

  /*
   * 3)
   */

  gdtr.address = (t_uint32)gdt;
  gdtr.size = (t_uint16)(PMODE_GDT_ENTRIES * sizeof(t_gdte));

  LGDT(gdtr);

  /*
   * 4)
   */

  bootloader_pmode_update_registers(PMODE_GDT_CORE_CS, PMODE_GDT_CORE_DS);

  /*
   * 5)
   */

  bootloader_pmode_enable();

  /*
   * 6)
   */

  memcpy(&init->machdep.gdtr, &gdtr, sizeof(t_gdtr));
}

/*                                                                 [cut] /k1 */
