/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane.../ia32/educational/include/architecture.h
 *
 * created       julien quintard   [thu jun  7 12:02:10 2007]
 * updated       julien quintard   [thu jun 28 22:54:39 2007]
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

#include <architecture/asm.h>
#include <architecture/debug.h>
#include <architecture/apic.h>
#include <architecture/misc.h>
#include <architecture/tss.h>
#include <architecture/pmode.h>
#include <architecture/gdt.h>
#include <architecture/idt.h>
#include <architecture/paging.h>

#include <architecture/context.h>
#include <architecture/interrupt.h>
#include <architecture/syscalls.h>
#include <architecture/virtual.h>

#endif
