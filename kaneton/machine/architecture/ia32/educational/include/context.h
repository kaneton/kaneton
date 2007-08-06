/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/task/context.h
 *
 * created       renaud voltz   [tue apr  4 22:01:00 2006]
 * updated       matthieu bucchianeri   [tue feb  6 19:49:02 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

#ifndef ARCHITECTURE_CONTEXT_H
#define ARCHITECTURE_CONTEXT_H	1

/*								[cut] k3 */

/*
 * ---------- macro functions -------------------------------------------------
 */

#define	IA32_SAVE_CONTEXT()						\
	 "	pushl %ebp				\n"		\
	 "	pushl %eax				\n"		\
	 "	pushl %ebx				\n"		\
	 "	pushl %ecx				\n"		\
	 "	pushl %edx				\n"		\
	 "	pushl %esi				\n"		\
	 "	pushl %edi				\n"		\
	 "	movl %cr3, %eax				\n"		\
	 "	pushl %eax				\n"		\
	 "	movl ia32_interrupt_pdbr, %eax		\n"		\
	 "	movl %eax, %cr3				\n"		\
	 "	pushw %ds				\n"		\
	 "	pushw %es				\n"		\
	 "	pushw %fs				\n"		\
	 "	pushw %gs				\n"		\
	 "	movl %esp, ia32_context			\n"		\
	 "	movw ia32_interrupt_ds, %ax		\n"		\
	 "	movw %ax, %ds				\n"		\
	 "	movw %ax, %es				\n"		\
	 "	movw %ax, %fs				\n"		\
	 "	movw %ax, %gs				\n"

#define IA32_RESTORE_CONTEXT()						\
	 "	movl $0, ia32_context			\n"		\
	 "	popw %gs				\n"		\
	 "	popw %fs				\n"		\
	 "	popw %es				\n"		\
	 "	popw %ds				\n"		\
	 "	popl %eax				\n"		\
	 "	movl %cr3, %ebx				\n"		\
	 "	cmp %eax, %ebx				\n"		\
	 "	je 1f					\n"		\
	 "	movl %eax, %cr3				\n"		\
	 "1:						\n"		\
	 "	popl %edi				\n"		\
	 "	popl %esi				\n"		\
	 "	popl %edx				\n"		\
	 "	popl %ecx				\n"		\
	 "	popl %ebx				\n"		\
	 "	popl %eax				\n"		\
	 "	popl %ebp				\n"

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * extended CPU capabilities.
 */

#define IA32_CAPS_MMX		(1 << 0)
#define IA32_CAPS_SSE		(1 << 1)

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * basic IA-32 context.
 */

typedef struct
{
  t_uint16	gs;
  t_uint16	fs;
  t_uint16	es;
  t_uint16	ds;
  t_uint32	cr3;
  t_uint32	edi;
  t_uint32	esi;
  t_uint32	edx;
  t_uint32	ecx;
  t_uint32	ebx;
  t_uint32	eax;
  t_uint32	ebp;
  t_uint32	code;
  t_uint32	eip;
  t_uint32	cs;
  t_uint32	eflags;
  t_uint32	esp;
  t_uint32	ss;
}		__attribute__ ((packed)) t_ia32_context;

/*
 * x87 FPU state.
 */

typedef struct
{
  t_uint16	fcw;
  t_uint16	reserved1;
  t_uint16	fsw;
  t_uint16	reserved2;
  t_uint16	ftw;
  t_uint16	reserved3;
  t_uint32	fpu_eip;
  t_uint16	fpu_cs;
  t_uint32	opcode:11;
  t_uint32	reserved4:5;
  t_uint32	fpu_ptr;
  t_uint16	fpu_ds;
  t_uint16	reserved5;
  t_uint8	st0[10];
  t_uint8	st1[10];
  t_uint8	st2[10];
  t_uint8	st3[10];
  t_uint8	st4[10];
  t_uint8	st5[10];
  t_uint8	st6[10];
  t_uint8	st7[10];
}		__attribute__ ((packed)) t_x87_state;

/*
 * x87 FPU, MMX and SSE state.
 */

typedef struct
{
  t_uint16	fcw;
  t_uint16	fsw;
  t_uint8	ftw;
  t_uint8	reserved1;
  t_uint16	fop;
  t_uint32	fpu_eip;
  t_uint16	fpu_cs;
  t_uint16	reserved2;
  t_uint32	fpu_ptr;
  t_uint16	fpu_ds;
  t_uint16	reserved3;
  t_uint32	mxcsr;
  t_uint32	mxcsr_mask;
  t_uint8	st0[10];
  t_uint8	reserved4[6];
  t_uint8	st1[10];
  t_uint8	reserved5[6];
  t_uint8	st2[10];
  t_uint8	reserved6[6];
  t_uint8	st3[10];
  t_uint8	reserved7[6];
  t_uint8	st4[10];
  t_uint8	reserved8[6];
  t_uint8	st5[10];
  t_uint8	reserved9[6];
  t_uint8	st6[10];
  t_uint8	reserved10[6];
  t_uint8	st7[10];
  t_uint8	reserved11[6];
  t_uint8	xmm0[16];
  t_uint8	xmm1[16];
  t_uint8	xmm2[16];
  t_uint8	xmm3[16];
  t_uint8	xmm4[16];
  t_uint8	xmm5[16];
  t_uint8	xmm6[16];
  t_uint8	xmm7[16];
  t_uint8	reserved12[224];
}		__attribute__ ((packed)) t_sse_state;

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * global variable pointing the context.
 */

extern t_ia32_context*	ia32_context;

/*
 * CPU capabilities from CPUID.
 */

extern t_uint32		ia32_cpucaps;

/*								[cut] /k3 */

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../context.c
 */

/*
 * ../context.c
 */

t_error			ia32_update_pdbr(i_task			tskid,
					 i_as			asid);

t_error			ia32_extended_context_init(void);

void			ia32_context_copy(t_ia32_context*		dst,
					  const t_ia32_context*		src);

t_error			ia32_clear_io_bitmap(i_task		tskid);

t_error			ia32_duplicate_io_bitmap(i_task		old,
						 i_task		new);

t_error			ia32_set_io_bitmap(i_task		tskid,
					   i_port		id,
					   t_uint8		width,
					   t_uint8		allow);

t_error			ia32_reset_iopl(void);

t_error			ia32_init_context(i_task		taskid,
					  i_thread		threadid);

t_error			ia32_duplicate_context(i_thread		old,
					       i_thread		new);

t_error			ia32_setup_context(i_thread		threadid,
					   t_vaddr		pc,
					   t_vaddr		sp);

t_error			ia32_status_context(i_thread		threadid,
					    t_vaddr*		pc,
					    t_vaddr*		sp);

t_error			ia32_init_switcher(void);

t_error			ia32_context_switch(i_thread		current,
					    i_thread		elected);

t_error			ia32_extended_context_switch(i_thread	current,
						     i_thread	elected);

t_error			ia32_push_args(i_thread			threadid,
				       void*			args,
				       t_vsize			size);


/*
 * eop
 */

#endif
