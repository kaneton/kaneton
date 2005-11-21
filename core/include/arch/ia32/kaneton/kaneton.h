/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * kaneton.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:23:41 2005   mycure
 * last update   Tue Nov 15 19:15:34 2005   mycure
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
#include <arch/ia32/kaneton/debug.h>
#include <arch/ia32/kaneton/init.h>
#include <arch/ia32/kaneton/segment.h>
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


/*
 * ../../../../kaneton/arch/ia32/kaneton/segment.c
 */

t_error			ia32_segment_init(t_fit			fit);

t_error			ia32_segment_clean(void);


/*
 * eop
 */

#endif
