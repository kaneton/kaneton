/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/pmode/pmode.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:15 2005]
 * updated       matthieu bucchianeri   [mon jan 30 23:59:44 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage protected mode.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students  must  write  here  function related  to  protected  mode:
 * entering, leaving etc.
 *
 * but  be careful: everything  that deals  with gdt  and ldt  must be
 * placed in the corresponding file, not here.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*                                                                  [cut] k2 */

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

/*                                                                 [cut] /k2 */

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
  CLI();

  asm volatile("movl %%cr0, %%eax\n\t"
	       "orw $1, %%ax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax");

  asm volatile("jmp pmode_enable_next\t\n"
	       "pmode_enable_next:"
	       );

  /* XXX STI(); */

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
