/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...ecture/ultrasparc/include/architecture.h
 *
 * created       matthieu bucchianeri   [sun jun 17 19:57:10 2007]
 * updated       matthieu bucchianeri   [sun jun 17 19:57:36 2007]
 */

#ifndef ARCHITECTURE_ARCHITECTURE_H
#define ARCHITECTURE_ARCHITECTURE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ___endian		ENDIAN_BIG
#define ___wordsz		WORDSZ_64

#define PAGESZ			8192

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/types.h>

#include <architecture/tlb.h>
#include <architecture/tick.h>

#include <architecture/virtual.h>

#endif
