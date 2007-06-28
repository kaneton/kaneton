/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ecture/ultrasparc/include/architecture.h
 *
 * created       matthieu bucchianeri   [sun jun 17 19:57:10 2007]
 * updated       julien quintard   [thu jun 28 22:55:04 2007]
 */

#ifndef ARCHITECTURE_ARCHITECTURE_H
#define ARCHITECTURE_ARCHITECTURE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ENDIAN			___kaneton$endian
#define WORDSZ			___kaneton$wordsz
#define PAGESZ			___kaneton$pagesz

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/types.h>

#include <architecture/tlb.h>
#include <architecture/tick.h>

#include <architecture/virtual.h>

#endif
