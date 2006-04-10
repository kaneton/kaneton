/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/libs/libia32/include/task/task.h
 *
 * created       renaud voltz   [tue apr  4 22:01:00 2006]
 * updated       renaud voltz   [tue apr  4 22:01:00 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

#ifndef IA32_TASK_TASK_H
#define IA32_TASK_TASK_H	1

/*								[cut] k4 */

/*
 * ---------- defines ---------------------------------------------------------
 */

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 *
 */

typedef struct
{
  t_uint16		cs;
  t_uint16		ds;
  t_uint16		es;
  t_uint16		fs;
  t_uint16		gs;
  t_uint16		ss;
  t_uint32		cr3;
  t_uint32		eip;
  t_uint32		eflags;
  t_uint32		edi;
  t_uint32		esi;
  t_uint32		ebp;
  t_uint32		esp;
  t_uint32		ebx;
  t_uint32		edx;
  t_uint32		ecx;
  t_uint32		eax;
}			__attribute__ ((packed)) t_ia32_context;

/*
 * save the running thread context before a context switch.
 *
 * steps:
 *
 * 1) push all general purpose registers.
 * 2) push eflags.
 * 3) push data segment register.
 * 4) save stack segment and esp registers.
 *
 */

#define CONTEXT_SAVE(_context_)						\
  asm volatile("pusha\n\t"						\
	       "pushf\n\t"						\
	       "subl $14,%%esp\n\t"					\
	       : "=m" (_context_)					\
	       :)

/*
 * restore the context of a thread to be run,
 *
 * steps:
 *
 * 1) load stack segment and esp registers.
 * 2) pop data segment register.
 * 3) pop eflags.
 * 4) pop all general purpose registers.
 *
 */

#define CONTEXT_RESTORE(_context_)					\
  asm volatile("movw %0,%%ss\n\t"					\
	       "movl %1,%%esp \n\t"					\
	       "popf\n\t"						\
	       "popa\n\t"						\
	       :							\
	       : "m" ((_context_)->ss), "m" ((_context_)->esp))

/*								[cut] /k4 */

#endif
