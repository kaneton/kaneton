/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ia32-virtual/bootloader.h
 *
 * created       julien quintard   [fri feb 11 02:23:53 2005]
 * updated       matthieu bucchianeri   [sun apr  2 23:53:42 2006]
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H		1

/*                                                                  [cut] k1 */

/*
 * ---------- macros ----------------------------------------------------------
 */

#define PMODE_GDT_ENTRIES	256

#define PMODE_BOOTLOADER_CS	0x1
#define PMODE_BOOTLOADER_DS	0x2

/*                                                                 [cut] /k1 */


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

void			bootloader_paging_init(void);


/*
 * pmode.c
 */

void			bootloader_pmode_init(void);


/*
 * eop
 */

#endif
