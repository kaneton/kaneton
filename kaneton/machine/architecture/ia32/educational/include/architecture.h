/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan.../ia32/educational/include/architecture.h
 *
 * created       julien quintard   [thu jun  7 12:02:10 2007]
 * updated       matthieu bucchianeri   [sat sep  1 13:24:57 2007]
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

#include <core/id.h>
#include <core/types.h>
#include <core/error.h>

#include <architecture/types.h>
#include <architecture/debug.h>

#include <architecture/generic/architecture.h>

#include <architecture/context.h>
#include <architecture/interrupt.h>
#include <architecture/syscalls.h>
#include <architecture/virtual.h>

#endif
