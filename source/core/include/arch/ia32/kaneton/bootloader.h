/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.h
 * 
 * path          /home/mycure/kaneton/core/include
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Sun Jun 19 22:02:00 2005   mycure
 */

#ifndef IA32_KANETON_BOOTLOADER_H
#define IA32_KANETON_BOOTLOADER_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/ia32/ia32/pmode.h>
#include <arch/ia32/ia32/paging.h>
#include <arch/ia32/services/cons.h>
#include <arch/ia32/ia32/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_gdte*			gdt;
  t_pde*			pd;
  t_cons			cons;
}				d_init;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * machdep traps
 */

#define		machdep_include_init()					\
  d_init			machdep

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../../../bootloader/arch/ia32/bootloader.c
 *      ../../../../bootloader/arch/ia32/cons.c
 *      ../../../../bootloader/arch/ia32/init.c
 *      ../../../../bootloader/arch/ia32/paging.c
 *      ../../../../bootloader/arch/ia32/pmode.c
 */

/*
 * ../../../../bootloader/arch/ia32/bootloader.c
 */

void			bootloader_error(void);

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi);


/*
 * ../../../../bootloader/arch/ia32/cons.c
 */

void			bootloader_cons_clear(void);

void			bootloader_cons_scroll(t_uint16		lines);

void			bootloader_cons_attr(t_uint8		attr);

int			bootloader_cons_print_char(char		c);

void			bootloader_cons_print_string(char*	string);

void			bootloader_cons_msg(char		indicator,
					    char*		fmt,
					    ...);

void			bootloader_cons_load(void);

int			bootloader_cons_init(void);


/*
 * ../../../../bootloader/arch/ia32/init.c
 */

void			bootloader_init_dump(void);

 * 1) allocates memory for the malloc() function. indeed the malloc() function;

 *    is installed. thus the malloc() function will be able to ask;

 *    so the malloc() function needs an amount of critical pages to work with;

void			bootloader_init_segments(void);

void			bootloader_init_regions(void);

t_paddr			bootloader_init_alloc(t_psize		size,
					      t_psize*		psize);

t_vaddr			bootloader_init_relocate(multiboot_info_t*	mbi);

				      nmodules * sizeof(t_module) +;

    (o_segment*)bootloader_init_alloc(nsegments * sizeof(o_segment),;

    (o_region*)bootloader_init_alloc(nregions * sizeof(o_region),;


/*
 * ../../../../bootloader/arch/ia32/paging.c
 */

void			bootloader_paging_dump_table(t_pte*	table,
						     t_opts	opts);

void			bootloader_paging_dump_directory(t_pde*	directory,
							 t_opts	opts);

void			bootloader_paging_enable(void);

void			bootloader_paging_init(void);

  pd = (t_pde*)bootloader_init_alloc(PAGING_NPDE * sizeof(t_pde), NULL);;

  memset(pd, 0x0, PAGING_NPDE * sizeof(t_pde));;

  pt0 = (t_pte*)bootloader_init_alloc(PAGING_NPTE * sizeof(t_pte), NULL);;

  memset(pt0, 0x0, PAGING_NPTE * sizeof(t_pte));;

	  pt = (t_pte*)bootloader_init_alloc(PAGING_NPTE * sizeof(t_pte),;

	  memset(pt, 0x0, PAGING_NPTE * sizeof(t_pte));;


/*
 * ../../../../bootloader/arch/ia32/pmode.c
 */

void			bootloader_pmode_gdt_dump(void);

void			bootloader_pmode_update_registers(t_uint16	kcs,
							  t_uint16	kds);

void			bootloader_pmode_enable(void);

void			bootloader_pmode_gdt_set(t_uint16	entry,
						 t_paddr	base,
						 t_psize	limit,
						 t_uint8	type,
						 t_uint8	flags);

void			bootloader_pmode_init(void);

  gdt = (t_gdte*)bootloader_init_alloc(PMODE_GDT_ENTRIES * sizeof(t_gdte),;

  memset(gdt, 0x0, PMODE_GDT_ENTRIES * sizeof(t_gdte));;

  gdtr.size = (t_uint16)(PMODE_GDT_ENTRIES * sizeof(t_gdte));;


/*
 * eop
 */

#endif
