/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/pmode/pmode.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:15 2005]
 * updated       matthieu bucchianeri   [wed dec 13 02:37:33 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage protected mode.
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

extern t_ia32_gdt	gdt;

/*                                                                 [cut] /k2 */

/*                                                                  [cut] k3 */

/*
 * global interrupt descriptor table
 */

extern t_ia32_idt		idt;

/*                                                                 [cut] /k3 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * initialises protected mode.
 *
 * steps:
 *
 * 1) copie gdt and idt from the init variable.
 * 2) enable protected mode.
 */

t_error			pmode_init(void)
{

  /*
   * 1)
   */

  memcpy(&gdt, &init->machdep.gdt, sizeof (t_ia32_gdt));

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

  asm volatile("jmp 1f\t\n"
	       "1:"
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
