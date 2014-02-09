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

#include <architecture/local.h>

/*
 * ---------- macro functions -------------------------------------------------
 */

// XXX better remove cpu local variables in this version!

#define	IA32_SAVE_CONTEXT()						\
	 "	pusha					\n"		\
	 "	movw %ds, %ax				\n"		\
	 "	pushl %eax				\n"		\
	 "	movl ia32_interrupt_pdbr, %eax		\n"		\
	 "	movl %eax, %cr3				\n"		\
	 "	movw ia32_interrupt_ds, %ax		\n"		\
	 "	movw %ax, %ds				\n"		\
	 "	movw %ax, %es				\n"		\
	 "	movw %ax, %fs				\n"		\
	 "	movw %ax, %gs				\n"		\
	 "	pushl %esp				\n"		\
	 "	pushl $ia32_local_jump_stack		\n"		\
	 "	call ia32_cpu_local_set			\n"		\
	 "	addl $8, %esp				\n"		\
	 "	pushl ia32_local_interrupt_stack	\n"		\
	 "	call ia32_cpu_local_get			\n"		\
	 "	addl $4, %esp				\n"		\
	 "	movl %eax, %esp				\n"

#define IA32_RESTORE_CONTEXT()						\
	 "	pushl ia32_local_jump_stack		\n"		\
	 "	call ia32_cpu_local_get			\n"		\
	 "	addl $4, %esp				\n"		\
	 "	movl %eax, %edx				\n"		\
	 "	pushl ia32_local_jump_pdbr		\n"		\
	 "	call ia32_cpu_local_get			\n"		\
	 "	addl $4, %esp				\n"		\
	 "	movl %cr3, %ebx				\n"		\
	 "	cmp %eax, %ebx				\n"		\
	 "	je 1f					\n"		\
	 "	movl %eax, %cr3				\n"		\
	 "1:						\n"		\
	 "	movl %edx, %esp				\n"		\
	 "	popl %eax				\n"		\
	 "	movw %ax, %ds				\n"		\
	 "	movw %ax, %es				\n"		\
	 "	movw %ax, %fs				\n"		\
	 "	movw %ax, %gs				\n"		\
	 "	popa					\n"

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * extended CPU capabilities.
 */

#define IA32_CAPS_MMX		(1 << 0)
#define IA32_CAPS_SSE		(1 << 1)

/*
 * context mask fields.
 */

#define IA32_CONTEXT_EAX	(1 << 0)
#define IA32_CONTEXT_EBX	(1 << 1)
#define IA32_CONTEXT_ECX	(1 << 2)
#define IA32_CONTEXT_EDX	(1 << 3)
#define IA32_CONTEXT_ESI	(1 << 4)
#define IA32_CONTEXT_EDI	(1 << 5)
#define IA32_CONTEXT_EBP	(1 << 6)
#define IA32_CONTEXT_ESP	(1 << 7)
#define IA32_CONTEXT_EIP	(1 << 8)
#define IA32_CONTEXT_EFLAGS	(1 << 9)
#define IA32_CONTEXT_CS		(1 << 10)
#define IA32_CONTEXT_DS		(1 << 11)
#define IA32_CONTEXT_SS		(1 << 12)


#define IA32_CONTEXT_FULL	0x1fff

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
  t_uint32	ds;
  t_uint32	edi;
  t_uint32	esi;
  t_uint32	ebp;
  t_uint32	orig_esp;
  t_uint32	ebx;
  t_uint32	edx;
  t_uint32	ecx;
  t_uint32	eax;
  t_uint32	error_code;
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
  t_uint16	ftw;
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
 * CPU capabilities from CPUID.
 */

extern t_uint32		ia32_cpucaps;

/*
 * stack switching addresses
 */

extern t_ia32_cpu_local	ia32_local_interrupt_stack;
extern t_ia32_cpu_local	ia32_local_jump_stack;
extern t_ia32_cpu_local	ia32_local_jump_pdbr;


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../context.c
 */

/*
 * ../context.c
 */

t_status		ia32_extended_context_init(void);

t_status		ia32_clear_io_bitmap(i_task		tskid);

t_status		ia32_duplicate_io_bitmap(i_task		old,
						 i_task		new);

t_status		ia32_set_io_bitmap(i_task		tskid,
					   i_port		id,
					   t_uint8		width,
					   t_uint8		allow);

t_status		ia32_reset_iopl(void);

t_status		ia32_init_context(i_task		taskid,
					  i_thread		threadid);

t_status		ia32_duplicate_context(i_thread		old,
					       i_thread		new);

t_status		ia32_setup_context(i_thread		threadid,
					   t_vaddr		pc,
					   t_vaddr		sp);

t_status		ia32_status_context(i_thread		threadid,
					    t_vaddr*		pc,
					    t_vaddr*		sp);

t_status		ia32_init_switcher(void);

t_status		ia32_context_ring0_stack(void);

t_status		ia32_context_switch(i_thread		current,
					    i_thread		elected);

t_status		ia32_extended_context_switch(i_thread	current,
						     i_thread	elected);

t_status		ia32_push_args(i_thread			threadid,
				       const void*		args,
				       t_vsize			size);

t_status		ia32_get_context(i_thread		thread,
					 t_ia32_context*	context);

t_status		ia32_set_context(i_thread		thread,
					 t_ia32_context*	context,
					 t_uint32		mask);


/*
 * eop
 */

#endif
