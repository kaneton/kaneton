/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/task/task.h
 *
 * created       renaud voltz   [tue apr  4 22:01:00 2006]
 * updated       matthieu bucchianeri   [sat jul  8 19:09:47 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

#ifndef IA32_TASK_TASK_H
#define IA32_TASK_TASK_H	1

/*								[cut] k4 */
#if 0
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


typedef struct
{
//  t_uint32	ldtr;
  t_uint32	cr3;
  t_uint32	gs;
  t_uint32	fs;
  t_uint32	es;
  t_uint32	ds;
  t_uint32	esi;
  t_uint32	edi;
  t_uint32	edx;
  t_uint32	ecx;
  t_uint32	ebx;
  t_uint32	eax;
  t_uint8	reserved3[0x14];
  t_uint32	reserved2;
  t_uint32	ebp;
  t_uint32	eip;
  t_uint32	cs;
  t_uint32	eflags;
  t_uint32	esp;
  t_uint32	ss;
}		__attribute__ ((packed)) t_ia32_context;

/*
 * save cpu register on the stack
 */

#define SAVE_CONTEXT							\
  "pushl %eax\n\t"							\
  "pushl %ebx\n\t"							\
  "pushl %ecx\n\t"							\
  "pushl %edx\n\t"							\
  "pushl %edi\n\t"							\
  "pushl %esi\n\t"							\
  "pushl %ds\n\t"							\
  "pushl %es\n\t"							\
  "pushl %fs\n\t"							\
  "pushl %gs\n\t"							\
  "movl %cr3, %eax\n\t"							\
  "pushl %eax\n\t"


/*
 * restore cpu register from the stack
 */

#define RESTORE_CONTEXT							\
  "popl %eax\n\t"							\
  "movl %eax, %cr3\n\t"							\
  "popl %gs\n\t"							\
  "popl %fs\n\t"							\
  "popl %es\n\t"							\
  "popl %ds\n\t"							\
  "popl %esi\n\t"							\
  "popl %edi\n\t"							\
  "popl %edx\n\t"							\
  "popl %ecx\n\t"							\
  "popl %ebx\n\t"							\
  "popl %eax\n\t"

extern t_ia32_context* context;

#endif
