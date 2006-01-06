/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/pmode.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:15 2005]
 * updated       matthieu bucchianeri   [fri dec 30 18:51:24 2005]
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
 * XXX plutot que de passer par gdt_refresh qui lit gdtr, p-e utiliser t_init?
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/*
 * initialises protected mode.
 *
 * steps:
 *
 * XXX
 */

t_error			pmode_init(void)
{
  gdt_refresh();

  pmode_enable();

#if (IA32_DEBUG & IA32_DEBUG_PMODE)
  pmode_test();
#endif

  paging_init(); // XXX remove me !

  return ERROR_NONE;
}

/*
 * enables protected mode by setting PE bit in CR0.
 */

t_error			pmode_enable(void)
{
  asm volatile("movl %%cr0, %%eax\n\t"
	       "orw $1, %%ax\n\t"
	       "movl %%eax, %%cr0\n\t"
	       :
	       :
	       : "%eax");

  return ERROR_NONE;
}

/*
 * sets code/data/stack segment registers.
 */

t_error			pmode_set_segment_registers(t_uint16	seg_code,
						    t_uint16	seg_data)
{
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

/*
 * test function
 */

void			pmode_test(void)
{
  t_uint16		seg1;
  t_uint16		seg2;
  t_segment		seg1des;

  gdt_size(NULL, &seg1);

  printf("current gdt size %d\n", seg1);

  seg1des.base = 0;
  seg1des.limit = 1024;
  seg1des.privilege = prvl_supervisor;
  seg1des.is_system = 0;
  seg1des.type.usr = type_code;

  gdt_reserve_segment(NULL, seg1des, &seg1);

  printf("reserved segment : %d\n", seg1);

  seg1des.base = 1456789;
  seg1des.limit = 1024 * 1024 * 3;
  seg1des.privilege = prvl_supervisor;
  seg1des.is_system = 0;
  seg1des.type.usr = type_data;

  gdt_reserve_segment(NULL, seg1des, &seg2);

  printf("reserved segment : %d\n", seg2);

  gdt_dump(NULL);

  gdt_delete_segment(NULL, seg2);

  seg1des.base = 4242;
  seg1des.limit = 256 * sizeof(t_ldte);
  seg1des.privilege = prvl_supervisor;
  seg1des.is_system = 1;
  seg1des.type.sys = type_ldt;

  gdt_add_segment(NULL, 42, seg1des);

  gdt_dump(NULL);

  gdt_delete_segment(NULL, 42);
  gdt_delete_segment(NULL, seg1);
}

/*                                                                 [cut] /k2 */
