/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/architecture/ia32/educational/pmode.c
 *
 * created       julien quintard   [sat jan  8 19:01:44 2011]
 * updated       julien quintard   [sun jan 16 13:42:19 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains functions for managing the ia32 protected mode.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init structure.
 */

extern s_init*			_init;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function enables the protected mode by setting the PE bit in CR0.
 */

t_status	architecture_pmode_enable(void)
{
  asm volatile("movl %%cr0, %%eax\n"
	       "orw $1, %%ax\n"
	       "movl %%eax, %%cr0\n"
	       :
	       :
	       : "%eax");

  asm volatile("jmp 1f\n"
	       "1:"
	       );

  MACHINE_LEAVE();
}

/*
 * this function sets up the protected mode.
 *
 * steps:
 *
 * 1) build a GDT descriptor based on the GDT prepared by the boot loader.
 * 2) import the descriptor in order to make it the current system's GDT.
 * 3) enable the protected mode.
 */

t_status	architecture_pmode_setup(void)
{
  as_gdt	gdt;

  /*
   * 1)
   */

  gdt.table = (void*)_init->machine.gdt;
  gdt.size = ARCHITECTURE_GDT_SIZE;

  /*
   * 2)
   */

  if (architecture_gdt_build(_init->machine.gdt,
			     ARCHITECTURE_GDT_SIZE * sizeof (at_gdte),
			     &gdt) != STATUS_OK)
    MACHINE_ESCAPE("unable to build the GDT");

  if (architecture_gdt_import(&gdt) != STATUS_OK)
    MACHINE_ESCAPE("unable to import the boot loader's GDT");

  /*
   * 3)
   */

  if (architecture_pmode_enable() != STATUS_OK)
    MACHINE_ESCAPE("unable to enable the protected mode");

  MACHINE_LEAVE();
}

/*
 * this function install the given segment registers by updating
 * the processor's segment registers.
 */

t_status	architecture_pmode_registers(t_uint16	code,
					     t_uint16	data)
{
  asm volatile("pushl %0\n\t"
	       "pushl $1f\n\t"
	       "lret\n\t"
	       "1:\n\t"
	       "movw %1, %%ax\n\t"
	       "movw %%ax, %%ds\n\t"
	       "movw %%ax, %%ss\n\t"
	       "movw %%ax, %%es\n\t"
	       "movw %%ax, %%fs\n\t"
	       "movw %%ax, %%gs\n\t"
	       :
	       : "g" (code), "g" (data)
	       : "memory", "%eax"
	       );

  MACHINE_LEAVE();
}
