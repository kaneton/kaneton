/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/include/libia32.h
 *
 * created       matthieu bucchianeri   [tue dec 20 13:58:56 2005]
 * updated       matthieu bucchianeri   [fri jan  6 11:52:59 2006]
 */

#ifndef LIBIA32_H
#define LIBIA32_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include "misc/asm.h"
#include "misc/isa.h"
#include "misc/multiboot.h"
#include "misc/stdarg.h"
#include "misc/types.h"
#include "paging/paging.h"
#include "pmode/pmode.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../pmode/pmode.c
 *      ../pmode/gdt.c
 *      ../pmode/ldt.c
 *      ../paging/paging.c
 *      ../paging/pd.c
 *      ../paging/pt.c
 *      ../paging/tlb.c
 *      ../paging/page.c
 *      ../interrupt/interrupt.c
 *      ../interrupt/idt.c
 */

/*
 * ../pmode/pmode.c
 */

t_error			pmode_init(void);

t_error			pmode_enable(void);

t_error			pmode_set_segment_registers(t_uint16	seg_code,
						    t_uint16	seg_data);

t_error			pmode_clean(void);


/*
 * ../pmode/gdt.c
 */

t_error			gdt_dump(t_gdt*       	dump_gdt);

t_error			gdt_size(t_gdt*		table,
				 t_uint16	*size);

t_error			gdt_build(t_uint16	entries,
				  t_paddr	base,
				  t_gdt*	gdt,
				  t_uint8	clear);

t_error			gdt_activate(t_gdt	new_gdt);

t_error			gdt_import(t_gdt*	gdt);

t_error			gdt_add_segment(t_gdt*		table,
					t_uint16	segment,
					t_segment	descriptor);

t_error			gdt_reserve_segment(t_gdt*	table,
					    t_segment	descriptor,
					    t_uint16*	segment);

t_error			gdt_get_segment(t_gdt*		table,
					t_uint16	index,
					t_segment*	segment);

t_error			gdt_delete_segment(t_gdt*	table,
					   t_uint16	segment);

t_error			gdt_build_selector(t_uint16	segment,
					   t_prvl	privilege,
					   t_uint16*	selector);


/*
 * ../pmode/ldt.c
 */

t_error			ldt_base(t_ldt*		table,
				 t_paddr*	addr);

t_error			ldt_size(t_ldt*		table,
				 t_uint16	*size);

t_error			ldt_dump(t_ldt*		table);

t_error			ldt_activate(t_ldt	table);

t_error			ldt_build(t_uint16	entries,
				  t_paddr	base,
				  t_ldt*	ldt,
				  t_uint8	clear);

t_error			ldt_destroy(t_ldt	*ldt);

t_error			ldt_add_segment(t_ldt*		table,
					t_uint16	segment,
					t_segment	descriptor);

t_error			ldt_reserve_segment(t_ldt*	table,
					    t_segment	descriptor,
					    t_uint16*	segment);

t_error			ldt_get_segment(t_ldt*		table,
					t_uint16	index,
					t_segment*	segment);

t_error			ldt_delete_segment(t_ldt*	table,
					   t_uint16	segment);

t_error			ldt_build_selector(t_ldt*	table,
					   t_uint16	segment,
					   t_prvl	privilege,
					   t_uint16*	selector);


/*
 * ../paging/paging.c
 */

t_error			paging_init(void);

t_error			paging_enable(void);

t_error			paging_clean(void);


/*
 * ../paging/pd.c
 */

t_error			pd_dump(t_directory*	dir);

t_error			pd_build(t_paddr	base,
				 t_directory*	directory,
				 t_uint8	clear);

t_error			pd_base(t_directory*		dir,
				t_paddr*		base);

t_error			pd_activate(t_directory		dir);

t_error			pd_add_table(t_directory*	dir,
				     t_uint16		entry,
				     t_table		table);

t_error			pd_get_table(t_directory*	dir,
				     t_uint16		entry,
				     t_table*		table);

t_error			pd_delete_table(t_directory*	dir,
					t_uint16	entry);


/*
 * ../paging/pt.c
 */

t_error			pt_dump(t_pte*	tab);

t_error			pt_build(t_paddr	base,
				 t_table*	table,
				 t_uint8	clear);

t_error			pt_add_page(t_table*	tab,
				    t_uint16	entry,
				    t_page	page);

t_error			pt_get_page(t_table*	tab,
				    t_uint16	entry,
				    t_page*	page);

t_error			pt_delete_page(t_table*	tab,
				       t_uint16	entry);


/*
 * ../paging/tlb.c
 */

t_error			tlb_invalidate(t_paddr	page);

t_error			tlb_flush(void);


/*
 * ../paging/page.c
 */

t_error		page_reserve(t_page*	page);


/*
 * ../interrupt/interrupt.c
 */


/*
 * ../interrupt/idt.c
 */


/*
 * eop
 */

#endif