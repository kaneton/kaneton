/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/pmode/pmode.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:15 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:39:53 2007]
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

#include <kaneton.h>

#include <architecture/architecture.h>

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

extern t_ia32_gdt	ia32_gdt;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * enables protected mode by setting PE bit in CR0.
 */

static void	ia32_pmode_enable(void)
{
  asm volatile("movl %%cr0, %%eax\n\t"
	       "orw $1, %%ax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax");

  asm volatile("jmp 1f\t\n"
	       "1:"
	       );
}

/*
 * initializes protected mode.
 *
 * steps:
 *
 * 1) copy gdt from the init variable.
 * 2) enable protected mode.
 */

t_status	ia32_pmode_init(void)
{

  /*
   * 1)
   */

  memcpy(&ia32_gdt, &init->machine.gdt, sizeof (t_ia32_gdt));

  /*
   * 2)
   */

  ia32_pmode_enable();

  return STATUS_OK;
}

/*
 * sets code/data/stack segment registers.
 */

t_status	ia32_pmode_set_segment_registers(t_uint16	seg_code,
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

  return STATUS_OK;
}

/*
 * ends protected mode.
 */

t_status	ia32_pmode_clean(void)
{
  return STATUS_OK;
}
