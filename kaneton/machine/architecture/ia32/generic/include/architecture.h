/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/lec_l/kanet...ture/ia32/generic/include/architecture.h
 *
 * created       julien quintard   [thu jun  7 12:02:10 2007]
 * updated       laurent lec   [thu mar 27 21:06:00 2008]
 */

#ifndef GENERIC_ARCHITECTURE_H
#define GENERIC_ARCHITECTURE_H	1

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

#include <architecture/generic/local.h>
#include <architecture/generic/asm.h>
#include <architecture/generic/apic.h>
#include <architecture/generic/misc.h>
#include <architecture/generic/tss.h>
#include <architecture/generic/pmode.h>
#include <architecture/generic/gdt.h>
#include <architecture/generic/idt.h>
#include <architecture/generic/paging.h>
#include <architecture/generic/ipi.h>
#include <architecture/generic/segment.h>
#include <architecture/generic/mapping.h>

#endif
