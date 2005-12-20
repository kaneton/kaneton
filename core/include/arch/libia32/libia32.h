/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/include/arch/libia32/libia32.h
 *
 * created       matthieu bucchianeri   [tue dec 20 13:58:56 2005]
 * updated       matthieu bucchianeri   [tue dec 20 14:08:20 2005]
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
 *      ../../../../libs/libia32/paging.c
 *      ../../../../libs/libia32/interrupt.c
 */

/*
 * ../../../../libs/libia32/pmode.c
 */

t_error			pmode_init(void);

t_error			pmode_clean(void);


/*
 * ../../../../libs/libia32/paging.c
 */


/*
 * ../../../../libs/libia32/interrupt.c
 */


/*
 * eop
 */

#endif
