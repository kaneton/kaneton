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

#ifndef IA32_TASK_TASK_H
#define IA32_TASK_TASK_H	1

/*								[cut] k3 */

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * extended CPU capabilities.
 */

#define IA32_CAPS_MMX		(1 << 0)
#define IA32_CAPS_SSE		(1 << 1)

#define	SAVE_CONTEXT()							\
	 "	pushl %ebp				\n"		\
	 "	pushl %eax				\n"		\
	 "	pushl %ebx				\n"		\
	 "	pushl %ecx				\n"		\
	 "	pushl %edx				\n"		\
	 "	pushl %esi				\n"		\
	 "	pushl %edi				\n"		\
	 "	movl %cr3, %eax				\n"		\
	 "	pushl %eax				\n"		\
	 "	movl interrupt_pdbr, %eax		\n"		\
	 "	movl %eax, %cr3				\n"		\
	 "	pushw %ds				\n"		\
	 "	pushw %es				\n"		\
	 "	pushw %fs				\n"		\
	 "	pushw %gs				\n"		\
	 "	movl %esp, context			\n"		\
	 "	movw interrupt_ds, %ax			\n"		\
	 "	movw %ax, %ds				\n"		\
	 "	movw %ax, %es				\n"		\
	 "	movw %ax, %fs				\n"		\
	 "	movw %ax, %gs				\n"

#define RESTORE_CONTEXT()						\
	 "	movl $0, context			\n"		\
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

extern t_ia32_context*	context;

/*
 * CPU capabilities from CPUID.
 */

extern t_uint32		cpucaps;

/*								[cut] /k3 */

#endif
