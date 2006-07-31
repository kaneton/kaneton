/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/task/tss.h
 *
 * created       renaud voltz   [mon apr 10 00:50:33 2006]
 * updated       matthieu bucchianeri   [sat jul 29 18:25:54 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

#ifndef IA32_TASK_TSS_H
#define IA32_TASK_TSS_H		1

/*								[cut] k4 */

/*
 * ---------- defines ---------------------------------------------------------
 */

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
}			__attribute__ ((packed)) t_ia32_tss;

/*								[cut] /k4 */

#endif
