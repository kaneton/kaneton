/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...hitecture/ia32/educational/include/tss.h
 *
 * created       renaud voltz   [mon apr 10 00:50:33 2006]
 * updated       julien quintard   [sat jan 29 10:53:34 2011]
 */

#ifndef ARCHITECTURE_TSS_H
#define ARCHITECTURE_TSS_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * this macro defines the number of pages required to contain the system's
 * TSS.
 */

#define ARCHITECTURE_TSS_SIZE		3 * ___kaneton$pagesz

/*
 * the two following macro can be used to ignore the SS0 and ESP0 TSS
 * attributes. these are particularly useful when context switching to a
 * ring0 thread since there is no need to specify a pile through SS0 and ESP0.
 */

#define ARCHITECTURE_TSS_SS0_NULL	0x0
#define ARCHITECTURE_TSS_ESP0_NULL	0x0

/*
 * this macro defines the location, i.e offset, of the IO map within the TSS.
 */

#define ARCHITECTURE_TSS_IO		0x68

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * the tss structure.
 */

typedef struct
{
  t_uint16		prev;
  t_uint16		prev_align;
  t_uint32		esp0;
  t_uint16		ss0;
  t_uint16		ss0_align;
  t_uint32		esp1;
  t_uint16		ss1;
  t_uint16		ss1_align;
  t_uint32		esp2;
  t_uint16		ss2;
  t_uint16		ss2_align;
  t_uint32		cr3;
  t_uint32		eip;
  t_uint32		eflags;
  t_uint32		eax;
  t_uint32		ecx;
  t_uint32		edx;
  t_uint32		ebx;
  t_uint32		esp;
  t_uint32		ebp;
  t_uint32		esi;
  t_uint32		edi;
  t_uint16		es;
  t_uint16		es_align;
  t_uint16		cs;
  t_uint16		cs_align;
  t_uint16		ss;
  t_uint16		ss_align;
  t_uint16		ds;
  t_uint16		ds_align;
  t_uint16		fs;
  t_uint16		fs_align;
  t_uint16		gs;
  t_uint16		gs_align;
  t_uint16		ldt;
  t_uint16		ldt_align;
  t_uint16		trap;
  t_uint16		io;
  t_uint8		io_bitmap[8192];
  t_uint8		io_end;
}			__attribute__ ((packed)) as_tss;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../tss.c
 */

/*
 * ../tss.c
 */

t_status		architecture_tss_build(t_vaddr		address,
					       as_tss**		tss);

t_status		architecture_tss_update(as_tss*		tss,
						t_uint16	ss0,
						t_uint32	esp0,
						t_uint32	io);

t_status		architecture_tss_activate(as_tss*	tss);


/*
 * eop
 */

#endif
