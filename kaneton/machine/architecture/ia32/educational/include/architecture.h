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
 * updated       julien quintard   [fri jan 14 14:19:45 2011]
 */

#ifndef ARCHITECTURE_ARCHITECTURE_H
#define ARCHITECTURE_ARCHITECTURE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * this macro defines the architecture's endianness i.e little endian.
 */

#define ___kaneton$endian		ENDIAN_LITTLE

/*
 * the frame size defines the fundamental unit of physical memory. in the
 * IA32 architecture, this unit is the byte.
 */

#define ___kaneton$framesz		1

/*
 * the page size defines the fundamental unit of virtual memory. because of
 * the way the IA32 architecture handles pagination through a hierarchy of
 * page directory, page tables and page frames, the fundamental unit of
 * such memory is the page i.e a 4096-byte of aligned memory.
 */

#define ___kaneton$pagesz		4096

/*
 * this macro defines the architecture's word size i.e 32-bit.
 */

#define ___kaneton$wordsz		32

/*
 * ---------- includes --------------------------------------------------------
 */

#include <architecture/asm.h>
#include <architecture/context.h>
#include <architecture/environment.h>
#include <architecture/gdt.h>
#include <architecture/handler.h>
#include <architecture/idt.h>
#include <architecture/io.h>
#include <architecture/linker.h>
#include <architecture/paging.h>
#include <architecture/pd.h>
#include <architecture/pmode.h>
#include <architecture/privilege.h>
#include <architecture/pt.h>
#include <architecture/register.h>
#include <architecture/segmentation.h>
#include <architecture/tlb.h>
#include <architecture/tss.h>
#include <architecture/types.h>

#endif
