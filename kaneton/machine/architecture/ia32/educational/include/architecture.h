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
 * updated       matthieu bucchianeri   [sat jun 16 19:30:44 2007]
 */

#ifndef ARCHITECTURE_ARCHITECTURE_H
#define ARCHITECTURE_ARCHITECTURE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ___endian		ENDIAN_LITTLE
#define ___wordsz		WORDSZ_32

#define PAGESZ			4096

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/types.h>

#include <architecture/asm.h>
#include <architecture/cons.h>
#include <architecture/debug.h>
#include <architecture/elf.h>
#include <architecture/apic.h>
#include <architecture/misc.h>
#include <architecture/context.h>
#include <architecture/tss.h>
#include <architecture/pmode.h>
#include <architecture/gdt.h>
#include <architecture/idt.h>
#include <architecture/paging.h>

#include <architecture/syscalls.h>
#include <architecture/virtual.h>

#endif
