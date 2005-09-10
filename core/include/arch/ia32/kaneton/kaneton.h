/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.h
 * 
 * path          /home/mycure/kaneton/core/include/arch/ia32/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:41 2005   mycure
 * last update   Sat Sep 10 15:09:12 2005   mycure
 */

#ifndef IA32_KANETON_KANETON_H
#define IA32_KANETON_KANETON_H	1

/*
 * ---------- defines ---------------------------------------------------------
 */

#define ___endian		ENDIAN_LITTLE
#define ___wordsz		WORDSZ_32

#define PAGESZ			4096

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/ia32/kaneton/as.h>
#include <arch/ia32/kaneton/bootloader.h>
#include <arch/ia32/kaneton/debug.h>
#include <arch/ia32/kaneton/init.h>
#include <arch/ia32/kaneton/stats.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../../../kaneton/arch/ia32/kaneton/as.c
 *      ../../../../kaneton/arch/ia32/kaneton/segment.c
 */

/*
 * ../../../../kaneton/arch/ia32/kaneton/as.c
 */

t_error			ia32_as_rsv(t_asid*			asid);

t_error			ia32_as_rel(t_asid			asid);


/*
 * ../../../../kaneton/arch/ia32/kaneton/segment.c
 */


/*
 * eop
 */

#endif
