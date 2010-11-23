/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/XYZ/...re/ia32/optimised/include/architecture.h
 *
 * created       julien quintard   [thu jun  7 12:02:10 2007]
 * updated       julien quintard   [tue nov 23 19:37:39 2010]
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
#define IA32_DEPENDENT

/*
 * ---------- includes --------------------------------------------------------
 */

#include <core/id.h>
#include <core/types.h>
#include <core/error.h>

#include <architecture/types.h>
#include <architecture/debug.h>

#include <architecture/context.h>
#include <architecture/interrupt.h>
#include <architecture/syscalls.h>
#include <architecture/virtual.h>

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

#endif
