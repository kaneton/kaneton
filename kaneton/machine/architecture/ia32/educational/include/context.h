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

/*
 * ---------- macro functions -------------------------------------------------
 */

/*						    [block::macros::context] */

// XXX better remove cpu local variables in this version!

// XXX dans le comment dire que les vars ont ete set et doivent etre
//     utilisees pour set le context proprement. tout ca a ete fait dans
//     ia32_context_switch() [context.c]:
//  ia32_local_jump_pdbr: PDBR du thread a sched
//  ia32_local_jmp_stack: address of the kernel stack of the resuming thread
//  + setting up the TSS to switch stack to the thread's kernel stack
//    on interrupt
//  + copying the iomap into the active TSS

// XXX noter a la fin qu'on recupere (cpu_local_get) la valeur de
//   interrupt_stack pour ensuet le branler comme ESP courant

// XXX 36(esp) -> exception error code

// XXX
// 1) save registers
// 2) push ds
// 3) save interrupted task's cr3 in local_jump_pdbr
// 4) retrieve error code in ebx
// 5) set the kernel as (interrupt_pdbr) as being the current cr3
// 6) set the kernel data segment (interrupt_ds) as being the current ds.
// 7) save esp in local_jump_stack
// 8) set la pile kernel speciale (local_interrupt_stack) as being esp.
// XXX
//
// DANS cette histoire du coup il n'est jamais question de la 'pile'
// d'un thread!
// -> dans context_switch() on set local_jump_stack comme etant a la
//    fin du thread's 'pile'. et on load le TSS dans cette pile, tout en
//    haut!
//
// XXX

#define	IA32_SAVE_CONTEXT()						\
  "	pusha							\n"	\
  "	movw %ds, %ax						\n"	\
  "	pushl %eax						\n"	\
  "	movl %cr3, %eax						\n"	\
  "	movl %eax, ia32_local_jump_pdbr				\n"	\
  "	movl 36(%esp), %ebx					\n"	\
  "	movl ia32_interrupt_pdbr, %eax				\n"	\
  "	movl %eax, %cr3						\n"	\
  "	movw ia32_interrupt_ds, %ax				\n"	\
  "	movw %ax, %ds						\n"	\
  "	movw %ax, %es						\n"	\
  "	movw %ax, %fs						\n"	\
  "	movw %ax, %gs						\n"	\
  "	movl %esp, ia32_local_jump_stack			\n"	\
  "	movl ia32_local_interrupt_stack, %esp			\n"

#define IA32_RESTORE_CONTEXT()						\
  "	movl ia32_local_jump_stack, %edx			\n"	\
  "	movl ia32_local_jump_pdbr, %eax				\n"	\
  "	movl %cr3, %ebx						\n"	\
  "	cmp %eax, %ebx						\n"	\
  "	je 1f							\n"	\
  "	movl %eax, %cr3						\n"	\
  "1:								\n"	\
  "	movl %edx, %esp						\n"	\
  "	popl %eax						\n"	\
  "	movw %ax, %ds						\n"	\
  "	movw %ax, %es						\n"	\
  "	movw %ax, %fs						\n"	\
  "	movw %ax, %gs						\n"	\
  "	popa							\n"

/*						 [endblock::macros::context] */

/*
 * ---------- macros ----------------------------------------------------------
 */

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
 * ---------- extern ----------------------------------------------------------
 */

/*
 * stack switching addresses
 */

extern t_uint32	ia32_local_interrupt_stack;
extern t_uint32	ia32_local_jump_stack;
extern t_uint32	ia32_local_jump_pdbr;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../context.c
 */

/*
 * ../context.c
 */

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

t_error			ia32_context_ring0_stack(void);

t_error			ia32_context_switch(i_thread		current,
					    i_thread		elected);

t_error			ia32_push_args(i_thread			threadid,
				       const void*		args,
				       t_vsize			size);

t_error			ia32_get_context(i_thread		thread,
					 t_ia32_context*	context);

t_error                 ia32_print_context(i_thread             thread);

t_error			ia32_set_context(i_thread		thread,
					 t_ia32_context*	context,
					 t_uint32		mask);


/*
 * eop
 */

#endif
