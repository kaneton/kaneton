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
 * last update   Sun Jun 19 20:35:27 2005   mycure
 */

#ifndef IA32_KANETON_H
#define IA32_KANETON_H		1

/*
 * ---------- defines ---------------------------------------------------------
 */

#define ___endian		ENDIAN_LITTLE
#define ___wordsz		WORDSZ_32

#define PAGESZ			4096

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/ia32/kaneton/bootloader.h>
#include <arch/ia32/kaneton/debug.h>
#include <arch/ia32/kaneton/init.h>

#endif
