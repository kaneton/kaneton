/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/pmode/pmode.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:15 2005]
 * updated       matthieu bucchianeri   [mon jan  9 12:13:11 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage protected mode.
 *
 * XXX d'apres le man intel, faudrait masquer les interruptions le temps du
 *     passage en mode protege (avec cli et sti). a regarder car ca foire
 *     quand je le fais.
 *
 * XXX de meme il faudrait reloader les registres de segments.
 *
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
 * global offset table.
 */

extern t_gdt		gdt;

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * initialises protected mode.
 *
 * steps:
 *
 * 1) copies gdt from the init variable.
 * 2) enable protected mode.
 */

t_error			pmode_init(void)
{

  /*
   * 1)
   */

  memcpy(&gdt, &init->machdep.gdt, sizeof (t_gdt));

  /*
   * 2)
   */

  pmode_enable();

  return ERROR_NONE;
}

/*
 * enables protected mode by setting PE bit in CR0.
 */

t_error			pmode_enable(void)
{
  t_uint16		kcs;
  t_uint16		kds;

  //CLI();

  asm volatile("movl %%cr0, %%eax\n\t"
	       "orw $1, %%ax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax");

  asm volatile("jmp pmode_enable_next\t\n"
	       "pmode_enable_next:"
	       );

  gdt_build_selector(1, prvl_supervisor, &kcs);
  gdt_build_selector(2, prvl_supervisor, &kds);

  //pmode_set_segment_registers(kcs, kds);

  //STI();

  return ERROR_NONE;
}

/*
 * sets code/data/stack segment registers.
 */

t_error			pmode_set_segment_registers(t_uint16	seg_code,
						    t_uint16	seg_data)
{
  asm volatile ("pushl %0\n\t"
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
		: "g" (seg_code), "g" (seg_data)
		: "memory", "%eax"
		);

  return ERROR_NONE;
}

/*
 * ends protected mode.
 */

t_error			pmode_clean(void)
{
  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
