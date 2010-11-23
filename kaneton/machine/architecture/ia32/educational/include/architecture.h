/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...itecture/ia32/NEW/include/architecture.h
 *
 * created       julien quintard   [thu jun  7 12:02:10 2007]
 * updated       julien quintard   [sat oct 16 14:02:17 2010]
 */

#ifndef ARCHITECTURE_ARCHITECTURE_H
#define ARCHITECTURE_ARCHITECTURE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ENDIAN			___kaneton$endian
#define WORDSZ			___kaneton$wordsz
#define PAGESZ			___kaneton$pagesz

#define IA32_CPU_MAX		16

// XXX
#define IA32_DEPENDENT

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/local.h>
#include <architecture/asm.h>
#include <architecture/apic.h>
#include <architecture/misc.h>
#include <architecture/tss.h>
#include <architecture/pmode.h>
#include <architecture/gdt.h>
#include <architecture/idt.h>
#include <architecture/paging.h>
#include <architecture/ipi.h>
#include <architecture/segment.h>
#include <architecture/mapping.h>
#include <architecture/context.h>
#include <architecture/debug.h>
#include <architecture/interrupt.h>
#include <architecture/spinlock.h>
#include <architecture/syscalls.h>
#include <architecture/types.h>
#include <architecture/virtual.h>
#include <architecture/linker.h>

#endif
