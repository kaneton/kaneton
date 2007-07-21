/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...ture/ia32/generic/include/architecture.h
 *
 * created       julien quintard   [thu jun  7 12:02:10 2007]
 * updated       matthieu bucchianeri   [sat jul 21 19:54:58 2007]
 */

#ifndef GENERIC_ARCHITECTURE_H
#define GENERIC_ARCHITECTURE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ENDIAN			___kaneton$endian
#define WORDSZ			___kaneton$wordsz
#define PAGESZ			___kaneton$pagesz

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/generic/asm.h>
#include <architecture/generic/apic.h>
#include <architecture/generic/misc.h>
#include <architecture/generic/tss.h>
#include <architecture/generic/pmode.h>
#include <architecture/generic/gdt.h>
#include <architecture/generic/idt.h>
#include <architecture/generic/paging.h>

#endif
