/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/arch/libia32/libia32.h
 *
 * created       matthieu bucchianeri   [tue dec 20 13:58:56 2005]
 * updated       matthieu bucchianeri   [tue dec 20 21:06:53 2005]
 */

#ifndef LIBIA32_H
#define LIBIA32_H

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/libia32/asm.h>
#include <arch/libia32/isa.h>
#include <arch/libia32/multiboot.h>
#include <arch/libia32/paging.h>
#include <arch/libia32/pmode.h>
#include <arch/libia32/stdarg.h>
#include <arch/libia32/types.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../../../libs/libia32/pmode.c
 *      ../../../../libs/libia32/gdt.c
 *      ../../../../libs/libia32/ldt.c
 *      ../../../../libs/libia32/paging.c
 *      ../../../../libs/libia32/pde.c
 *      ../../../../libs/libia32/pte.c
 *      ../../../../libs/libia32/tlb.c
 *      ../../../../libs/libia32/interrupt.c
 *      ../../../../libs/libia32/idt.c
 */

/*
 * ../../../../libs/libia32/pmode.c
 */

t_error			pmode_init(void);

t_error			pmode_enable(void);

t_error			pmode_set_segment_registers(t_uint16	seg_code,
						    t_uint16	seg_data);

t_error			pmode_clean(void);


/*
 * ../../../../libs/libia32/gdt.c
 */

t_error			gdt_build(t_uint16	entries);


/*
 * ../../../../libs/libia32/ldt.c
 */


/*
 * ../../../../libs/libia32/paging.c
 */

t_error			paging_init(void);

t_error			paging_enable(void);

t_error			paging_clean(void);


/*
 * ../../../../libs/libia32/pde.c
 */


/*
 * ../../../../libs/libia32/pte.c
 */


/*
 * ../../../../libs/libia32/tlb.c
 */


/*
 * ../../../../libs/libia32/interrupt.c
 */


/*
 * ../../../../libs/libia32/idt.c
 */


/*
 * eop
 */

#endif
