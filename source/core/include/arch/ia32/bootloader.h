/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.h
 * 
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:53 2005   mycure
 * last update   Thu Jun 16 15:59:53 2005   mycure
 */

#ifndef IA32_BOOTLOADER_H
#define IA32_BOOTLOADER_H	1

/*
 * prototypes:     ../../../bootloader/arch/ia32/bootloader.c
 *                 ../../../bootloader/arch/ia32/cons.c
 *                 ../../../bootloader/arch/ia32/init.c
 *                 ../../../bootloader/arch/ia32/paging.c
 *                 ../../../bootloader/arch/ia32/pmode.c
 */

/*
 * ../../../bootloader/arch/ia32/bootloader.c
 */

void			bootloader_error(void);


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
 * ../../../bootloader/arch/ia32/init.c
 */

void			init_dump(void);


void			init_segments(void);


void			init_regions(void);


t_paddr			init_alloc(t_psize			size,
				   t_psize*			psize);


t_vaddr			init_relocate(multiboot_info_t*		mbi);


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
 * eop
 */

#endif
