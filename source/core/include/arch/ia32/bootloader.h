/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.h
 * 
 * path          /home/mycure/kaneton/core/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Mon Jun 13 14:39:12 2005   mycure
 */

#ifndef IA32_BOOTLOADER_H
#define IA32_BOOTLOADER_H	1

#ifdef ___bootloader

/*
 * prototypes:     ../../../bootloader/arch/ia32/bootloader.c
 *                 ../../../bootloader/arch/ia32/cons.c
 *                 ../../../bootloader/arch/ia32/paging.c
 *                 ../../../bootloader/arch/ia32/pmode.c
 *                 ../../../bootloader/arch/ia32/segment.c
 */

/*
 * ../../../bootloader/arch/ia32/bootloader.c
 */

void			bootloader_error(void);


void			bootloader_segments(void);


void			bootloader_regions(void);


t_paddr			bootloader_alloc(t_psize		size,
					 t_psize*		psize);


t_vaddr			bootloader_relocate(multiboot_info_t*	mbi);


void			bootloader_dump(void);


int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi);


/*
 * ../../../bootloader/arch/ia32/cons.c
 */

void			cons_clear(void);


void			cons_scroll(t_uint16			lines);


void			cons_attr(t_uint8			attr);


int			cons_print_char(char			c);


void			cons_print_string(char*			string);


void			cons_msg(char				indicator,
				 char*				fmt,
				 ...);


void			cons_load(void);


int			cons_init(void);


/*
 * ../../../bootloader/arch/ia32/paging.c
 */

void			paging_dump_table(t_pte*		table,
					  t_opts		opts);


void			paging_dump_directory(t_pde*		directory,
					      t_opts		opts);


void			paging_enable(void);


void			paging_init(void);


/*
 * ../../../bootloader/arch/ia32/pmode.c
 */

void			pmode_gdt_dump(void);


void			pmode_update_registers(t_uint16		gdt_kernel_cs,
					       t_uint16		gdt_kernel_ds);


void			pmode_enable(void);


void			pmode_gdt_set(t_uint16			entry,
				      t_paddr			base,
				      t_psize			limit,
				      t_uint8			type,
				      t_uint8			flags);


void			pmode_init(void);


/*
 * ../../../bootloader/arch/ia32/segment.c
 */

void			segment_init(void);

/*
 * eop
 */

#endif

#endif
