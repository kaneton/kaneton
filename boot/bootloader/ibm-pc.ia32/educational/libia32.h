/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...loader/ibm-pc.ia32/educational/libia32.h
 *
 * created       matthieu bucchianeri   [mon jun 25 23:02:50 2007]
 * updated       matthieu bucchianeri   [thu jun 28 14:54:25 2007]
 */

#ifndef BOOTLOADER_LIBIA32_H
#define BOOTLOADER_LIBIA32_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

#define IA32_PD_MAX_ENTRIES	1024
#define IA32_PT_MAX_ENTRIES	1024
#define IA32_PD_NOTCACHED	0
#define IA32_PD_CACHED		1
#define IA32_PD_WRITETHROUGH	0
#define IA32_PD_WRITEBACK	1

#define IA32_PT_NOTCACHED	0
#define IA32_PT_CACHED		1
#define IA32_PT_WRITETHROUGH	0
#define IA32_PT_WRITEBACK	1
#define IA32_PT_READONLY	0
#define IA32_PT_WRITABLE	1
#define IA32_PT_PRIVILEGED	0
#define IA32_PT_USER		1

#define IA32_PG_NOTCACHED	0
#define IA32_PG_CACHED		1
#define IA32_PG_WRITETHROUGH	0
#define IA32_PG_WRITEBACK	1
#define IA32_PG_READONLY	0
#define IA32_PG_WRITABLE	1
#define IA32_PG_PRIVILEGED	0
#define IA32_PG_USER		1
#define IA32_PG_NONGLOBAL	0
#define IA32_PG_GLOBAL		1

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	libia32.c
 */

/*
 * libia32.c
 */

t_error		pmode_set_segment_registers(t_uint16	seg_code,
					    t_uint16	seg_data);

t_error			gdt_build(t_uint16		entries,
				  t_paddr		base,
				  t_ia32_gdt*		gdt,
				  t_uint8		clear);

t_error			gdt_activate(t_ia32_gdt		new_gdt);

t_error			gdt_add_segment(t_ia32_gdt*	table,
					t_uint16	segment,
					t_ia32_segment	descriptor);

t_error			gdt_build_selector(t_uint16	segment,
					   t_ia32_prvl	privilege,
					   t_uint16*	selector);

t_error			pd_build(t_paddr			base,
				 t_ia32_directory*		directory,
				 t_uint8			clear);

t_error			pd_activate(t_ia32_directory	dir,
				    t_uint32		cached,
				    t_uint32		writeback);

t_error			pd_add_table(t_ia32_directory*	dir,
				     t_uint16		entry,
				     t_ia32_table	table);

t_error			pd_get_table(t_ia32_directory*	dir,
				     t_uint16		entry,
				     t_ia32_table*	table);

t_error			pt_build(t_paddr		base,
				 t_ia32_table*		table,
				 t_uint8		clear);

t_error			pt_add_page(t_ia32_table*	tab,
				    t_uint16		entry,
				    t_ia32_page		page);


/*
 * eop
 */

#endif
