/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/arch/ia32-virtual/ia32/ia32.h
 *
 * created       julien quintard   [sat dec 17 17:12:43 2005]
 * updated       julien quintard   [sun dec 18 18:55:50 2005]
 */

#ifndef IA32_IA32_IA32_H
#define IA32_IA32_IA32_H	1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/machdep/ia32/asm.h>
#include <arch/machdep/ia32/isa.h>
#include <arch/machdep/ia32/multiboot.h>
#include <arch/machdep/ia32/paging.h>
#include <arch/machdep/ia32/pmode.h>
#include <arch/machdep/ia32/stdarg.h>
#include <arch/machdep/ia32/types.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../../../kaneton/arch/machdep/ia32/pmode.c
 *      ../../../../kaneton/arch/machdep/ia32/paging.c
 */

/*
 * ../../../../kaneton/arch/machdep/ia32/pmode.c
 */

t_error			pmode_gdt_dump(void);

t_error			pmode_update_registers(t_uint16		kcs,
					       t_uint16		kds);

t_error			pmode_enable(void);

t_error			pmode_gdt_set(t_uint16			entry,
				      t_paddr			base,
				      t_psize			limit,
				      t_uint8			type,
				      t_uint8			flags);

t_error			pmode_init(void);

t_error			pmode_clean(void);


/*
 * ../../../../kaneton/arch/machdep/ia32/paging.c
 */


/*
 * eop
 */

#endif
