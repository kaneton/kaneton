/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/include/task/task.h
 *
 * created       renaud voltz   [tue apr  4 22:01:00 2006]
 * updated       matthieu bucchianeri   [wed jul 26 12:45:53 2006]
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
 * ---------- macros ----------------------------------------------------------
 */

/*
 * extended CPU capabilities.
 */

#define IA32_CAPS_MMX		(1 << 0)
#define IA32_CAPS_SSE		(1 << 1)

/*
 * update the global variable used for pointing saved context.
 */

#define UPDATE_CONTEXT_PTR						\
  asm volatile("movl %%esp, %0\n\t"					\
	       : "=g" (context))

/*
 * get the x87 state pointer.
 */

#define X87_STATE()							\
  (t_x87_state*)((t_uint8*)context - 108)

/*
 * get the x87/mmx/sse state pointer.
 */

#define SSE_STATE()							\
  (t_sse_state*)(((t_uint32)context - 512) & 0xFFFFFFF0)

/*
 * save cpu registers on the stack
 */

#define SAVE_CONTEXT()							\
  asm volatile("pushl %eax\n\t"						\
	       "pushl %ebx\n\t"						\
	       "pushl %ecx\n\t"						\
	       "pushl %edx\n\t"						\
	       "pushl %edi\n\t"						\
	       "pushl %esi\n\t"						\
	       "pushl %ds\n\t"						\
	       "pushl %es\n\t"						\
	       "pushl %fs\n\t"						\
	       "pushl %gs\n\t"						\
	       "movl %cr3, %eax\n\t"					\
	       "pushl %eax\n\t");					\
  UPDATE_CONTEXT_PTR;							\
  if (cpucaps & IA32_CAPS_SSE)						\
    asm volatile("movl %%esp, %%eax\n\t"				\
		 "subl $512, %%esp\n\t"					\
		 "andw $0xFFF0, %%sp\n\t"				\
		 "fxsave (%%esp)\n\t"					\
		 "pushl %%eax"						\
		 :							\
		 :							\
		 : "%eax");						\
  else									\
    asm volatile("subl $108, %esp\n\t"					\
		 "fsave (%esp)")

/*
 * restore cpu registers from the stack
 */

#define RESTORE_CONTEXT()						\
  context = NULL;							\
  if (cpucaps & IA32_CAPS_SSE)						\
    asm volatile("popl %eax\n\t"					\
		 "fxrstor (%esp)\n\t"					\
		 "movl %eax, %esp");					\
  else									\
    asm volatile("frstor (%esp)\n\t"					\
		 "addl $108, %esp\n\t");				\
  asm volatile("popl %eax\n\t"						\
	       "movl %eax, %cr3\n\t"					\
	       "popl %gs\n\t"						\
	       "popl %fs\n\t"						\
	       "popl %es\n\t"						\
	       "popl %ds\n\t"						\
	       "popl %esi\n\t"						\
	       "popl %edi\n\t"						\
	       "popl %edx\n\t"						\
	       "popl %ecx\n\t"						\
	       "popl %ebx\n\t"						\
	       "popl %eax\n\t")

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * basic IA-32 context.
 */

typedef struct
{
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
  t_uint8	reserved1[0x14];
  t_uint32	reserved2;
  t_uint32	ebp;
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

#endif
