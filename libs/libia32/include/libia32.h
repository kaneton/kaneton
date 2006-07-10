/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/libia32.h
 *
 * created       matthieu bucchianeri   [tue dec 20 13:58:56 2005]
 * updated       matthieu bucchianeri   [mon jan 30 23:15:42 2006]
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
#include "pmode/gdt.h"
#include "pmode/idt.h"
#include "interrupt/interrupt.h"
#include "interrupt/pic.h"
#include "time/pit.h"
#include "task/task.h"
#include "task/tss.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../pmode/pmode.c
 *      ../pmode/gdt.c
 *      ../pmode/ldt.c
 *      ../pmode/idt.c
 *      ../paging/paging.c
 *      ../paging/pd.c
 *      ../paging/pt.c
 *      ../paging/tlb.c
 *      ../interrupt/pic.c
 *      ../interrupt/interrupt.c
 *	../time/timer.c
 *	../task/task.c
 *      ../task/tss.c
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

t_error			gdt_dump(t_ia32_gdt*			dump_gdt);

t_error			gdt_size(t_ia32_gdt*			table,
				 t_uint16			*size);

t_error			gdt_build(t_uint16			entries,
				  t_paddr			base,
				  t_ia32_gdt*			gdt,
				  t_uint8			clear);

t_error			gdt_activate(t_ia32_gdt			new_gdt);

t_error			gdt_import(t_ia32_gdt*			gdt);

t_error			gdt_add_segment(t_ia32_gdt*		table,
					t_uint16		segment,
					t_ia32_segment		descriptor);

t_error			gdt_reserve_segment(t_ia32_gdt*		table,
					    t_ia32_segment	descriptor,
					    t_uint16*		segment);

t_error			gdt_get_segment(t_ia32_gdt*		table,
					t_uint16		index,
					t_ia32_segment*		segment);

t_error			gdt_delete_segment(t_ia32_gdt*		table,
					   t_uint16		segment);

t_error			gdt_build_selector(t_uint16		segment,
					   t_ia32_prvl		privilege,
					   t_uint16*		selector);


/*
 * ../pmode/ldt.c
 */

t_error			ldt_base(t_ia32_ldt*			table,
				 t_paddr*			addr);

t_error			ldt_size(t_ia32_ldt*			table,
				 t_uint16			*size);

t_error			ldt_dump(t_ia32_ldt*			table);

t_error			ldt_activate(t_ia32_ldt			table);

t_error			ldt_build(t_uint16			entries,
				  t_paddr			base,
				  t_ia32_ldt*			ldt,
				  t_uint8			clear);

t_error			ldt_destroy(t_ia32_ldt			*ldt);

t_error			ldt_add_segment(t_ia32_ldt*		table,
					t_uint16		segment,
					t_ia32_segment		descriptor);

t_error			ldt_reserve_segment(t_ia32_ldt*		table,
					    t_ia32_segment	descriptor,
					    t_uint16*		segment);

t_error			ldt_get_segment(t_ia32_ldt*		table,
					t_uint16		index,
					t_ia32_segment*		segment);

t_error			ldt_delete_segment(t_ia32_ldt*		table,
					   t_uint16		segment);

t_error			ldt_build_selector(t_ia32_ldt*		table,
					   t_uint16		segment,
					   t_ia32_prvl		privilege,
					   t_uint16*		selector);


/*
 * ../pmode/idt.c
 */

t_error			idt_dump(t_ia32_idt*			table);

t_error			idt_size(t_ia32_idt*			table,
				 t_uint16			*size);

t_error			idt_build(t_uint16			entries,
				  t_paddr			base,
				  t_uint8			clear,
				  t_ia32_idt*			table);

t_error			idt_activate(t_ia32_idt*		table);

t_error			idt_import(t_ia32_idt*			table);

t_error			idt_add_gate(t_ia32_idt*		table,
				     t_uint16			index,
				     t_ia32_gate		gate);

t_error			idt_get_gate(t_ia32_idt*		table,
				     t_uint16			index,
				     t_ia32_gate*		gate);

t_error			idt_delete_gate(t_ia32_idt*		table,
					t_uint16		gate_id);


/*
 * ../paging/paging.c
 */

t_error			paging_init(void);

t_error			paging_enable(void);

t_error			paging_clean(void);


/*
 * ../paging/pd.c
 */

t_error			pd_dump(t_ia32_directory*		dir);

t_error			pd_build(t_paddr			base,
				 t_ia32_directory*		directory,
				 t_uint8			clear);

t_error			pd_base(t_ia32_directory*		dir,
				t_paddr*			base);

t_error			pd_activate(t_ia32_directory		dir);

t_error			pd_add_table(t_ia32_directory*		dir,
				     t_uint16			entry,
				     t_ia32_table		table);

t_error			pd_get_table(t_ia32_directory*		dir,
				     t_uint16			entry,
				     t_ia32_table*		table);

t_error			pd_delete_table(t_ia32_directory*	dir,
					t_uint16		entry);


/*
 * ../paging/pt.c
 */

t_error			pt_dump(t_ia32_pte*			tab);

t_error			pt_build(t_paddr			base,
				 t_ia32_table*			table,
				 t_uint8			clear);

t_error			pt_add_page(t_ia32_table*		tab,
				    t_uint16			entry,
				    t_ia32_page			page);

t_error			pt_get_page(t_ia32_table*		tab,
				    t_uint16			entry,
				    t_ia32_page*		page);

t_error			pt_delete_page(t_ia32_table*		tab,
				       t_uint16			entry);


/*
 * ../paging/tlb.c
 */

t_error			tlb_invalidate(t_paddr			page);

t_error			tlb_flush(void);


/*
 * ../interrupt/pic.c
 */

t_error			pic_init(void);

t_error			pic_enable_irq(t_uint8			irq);

t_error			pic_disable_irq(t_uint8			irq);

t_error			pic_acknowledge(t_uint8			irq);


/*
 * ../interrupt/interrupt.c
 */

t_error			interrupt_add(t_uint32			nr,
				      t_ia32_prvl	       	privilege,
				      t_ia32_interrupt_prehandler prehandler);

t_error			interrupt_set_handler(t_uint32			nr,
					      t_ia32_interrupt_handler	handler);

t_error			interrupt_init(void);

void			exception_wrapper(t_uint32		nr,
					  t_uint32		error_code);

void			irq_wrapper(t_uint32			nr);


/*
 * ../time/timer.c
 */

t_error			pit_init(t_uint32			frequency);


/*
 * ../task/task.c
 */

t_error			context_store(t_ia32_context*		context);

t_error			context_dump(t_ia32_context*		context);

void fun1();

void fun2();

void chiche_ctx(t_uint32 id);

void ctx_test();


/*
 * ../task/tss.c
 */

t_error			tss_load(t_ia32_tss*			tss_new,
				 t_uint16			ss,
				 t_uint32			esp,
				 t_uint32			io);

t_error			tss_init(t_ia32_tss*			base);


/*
 * eop
 */

#endif
