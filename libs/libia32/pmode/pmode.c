/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/pmode/pmode.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:15 2005]
 * updated       matthieu bucchianeri   [fri jan  6 15:05:16 2006]
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
 * 3) runs some tests if needed.
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

  /*
   * 3)
   */

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

/*
 * test function
 */

void			pmode_test(void)
{
  t_uint16		seg1;
  t_uint16		seg2;
  t_segment		seg1des;
  void			*ptr;
  int			r;
  t_uint16		es, fs;

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

  ptr = malloc(8);

  seg1des.base = (t_paddr)ptr;
  seg1des.limit = 8;
  seg1des.is_system = 0;
  seg1des.type.usr = type_data;

  gdt_reserve_segment(NULL, seg1des, &seg1);

  seg1des.base += 4;
  seg1des.limit = 4;

  gdt_reserve_segment(NULL, seg1des, &seg2);

  gdt_build_selector(seg1, prvl_supervisor, &es);
  gdt_build_selector(seg2, prvl_supervisor, &fs);

  asm volatile("movw %1, %%ax\t\n"
	       "movw %%ax, %%es\t\n"
	       "movl $41424344, %%es:(4)\t\n"
	       "movw %2, %%ax\t\n"
	       "movw %%ax, %%fs\t\n"
	       "movl %%fs:(0), %0\t\n"
	       : "=r" (r)
	       : "g" (es), "g" (fs)
	       : "%eax");

  if (r != 41424344)
    printf("error in segments !\n");
  printf("r = %d\n", r);

  free(ptr);

  gdt_delete_segment(NULL, seg1);
  gdt_delete_segment(NULL, seg2);
}

/*                                                                 [cut] /k2 */
