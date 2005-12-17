/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Tue Aug 30 14:02:52 2005   mycure
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H		1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      bootloader.c
 *      cons.c
 *      init.c
 *      paging.c
 *      pmode.c
 */

/*
 * bootloader.c
 */

void			bootloader_error(void);

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi);


/*
 * cons.c
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
 * init.c
 */

void			bootloader_init_dump(void);

void			bootloader_init_segments(void);

void			bootloader_init_regions(void);

t_paddr			bootloader_init_alloc(t_psize		size,
					      t_psize*		psize);

t_vaddr			bootloader_init_relocate(multiboot_info_t*	mbi);


/*
 * paging.c
 */

void			bootloader_paging_dump_table(t_pte*	table,
						     t_opts	opts);

void			bootloader_paging_dump_directory(t_pde*	directory,
							 t_opts	opts);

void			bootloader_paging_enable(void);

void			bootloader_paging_init(void);


/*
 * pmode.c
 */

void			bootloader_pmode_update_registers(t_uint16	kcs,
							  t_uint16	kds);

void			bootloader_pmode_enable(void);

void			bootloader_pmode_gdt_set(t_uint16	entry,
						 t_paddr	base,
						 t_psize	limit,
						 t_uint8	type,
						 t_uint8	flags);

void			bootloader_pmode_init(void);


/*
 * eop
 */

#endif
