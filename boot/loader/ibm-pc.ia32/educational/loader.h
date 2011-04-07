/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-virtual/bootloader.h
 *
 * created       julien quintard   [fri feb 11 02:23:53 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:16:13 2007]
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H		1

#define		machine_data(_object_)					\
  machine_data_##_object_()

#define		machine_data_init()					\
  struct								\
  {									\
    t_ia32_gdt			gdt;					\
    t_ia32_directory		pd;					\
  }

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include "types.h"
#include "init.h"

#include "multiboot.h"
#include "isa.h"
#include "elf.h"
#include "libia32.h"
#include "libc.h"
#include "cons.h"

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IA32_PMODE_GDT_ENTRIES		256

#define IA32_PMODE_BOOTLOADER_CS	0x1
#define IA32_PMODE_BOOTLOADER_DS	0x2

#define PERM_READ			(1 << 0)
#define PERM_WRITE			(1 << 1)
#define PERM_EXEC			(1 << 2)

#define SEGMENT_OPTION_NONE		(1 << 0)
#define SEGMENT_OPTION_SYSTEM		(1 << 1)

#define REGION_OPT_NONE			0
#define REGION_OPT_FORCE		(1 << 0)

#define PAGING_PTS			8

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      loader.c
 *      cons.c
 *      init.c
 *      paging.c
 *      pmode.c
 */

/*
 * loader.c
 */

void			bootloader_error(void);

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi);


/*
 * cons.c
 */

void			bootloader_cons_clear(void);

void			bootloader_cons_scroll(t_uint16		lines);

void			bootloader_cons_attr(t_sint8		attr);

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

void			faultcatcher_init(void);

#endif
