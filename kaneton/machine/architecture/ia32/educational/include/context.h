/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...cture/ia32/educational/include/context.h
 *
 * created       renaud voltz   [tue apr  4 22:01:00 2006]
 * updated       julien quintard   [mon feb  7 16:19:23 2011]
 */

#ifndef ARCHITECTURE_CONTEXT_H
#define ARCHITECTURE_CONTEXT_H	1

/*
 * ---------- macro functions -------------------------------------------------
 */

/*
 * this macro-function saves the context of the thread which had just
 * been interrupted.
 *
 * note that at this point, the stack in use is the thread's pile i.e ring0
 * stack; except for the kernel threads since there is no change in privilege.
 *
 * steps:
 *
 * 1) push all the registers on the current stack i.e the pile, hence
 *    saving their values. this set of values correspond to the as_context
 *    structure.
 * 2) push the DS - Data Segment selector.
 * 3) save the current CR3 i.e PDBR in _architecture.thread.pdbr.
 * 4) retrieve the error code. note that this code is not always present
 *    as only some exceptions provide them. however, the code is always
 *    saved in EBX et it is left to the handler shells to decide whether
 *    it is relevent to the current interrupt i.e it would for example be
 *    ignored for IRQs.
 * 5) put the kernel's PDBR (_architecture.kernel.pdbr) in EAX. if the
 *    kernel PDBR is equal to the PDBR of the interrupted thread
 *    (_architecture.thread.pdbr), do nothing. otherwise, load the kernel
 *    PDBR by changing the CPU's CR3.
 * 6) update the segment selectors DS, ES, FS et GS with the kernel
 *    data selector since the system is now running in the kernel environment.
 * 7) save the current stack pointer i.e ESP in _architecture.thread.pile.esp.
 * 8) finally, use the KIS - Kernel Interrupt Stack by setting ESP with
 *    the value contained in _architecture.kernel.kis.esp.
 */

#define	ARCHITECTURE_CONTEXT_SAVE()					\
  asm (									\
       /*								\
	* 1)								\
	*/								\
									\
       "pusha							\n"	\
									\
       /*								\
	* 2)								\
	*/								\
									\
       "movw %ds, %ax						\n"	\
       "pushl %eax						\n"	\
									\
       /*								\
	* 3)								\
	*/								\
									\
       "movl %cr3, %eax						\n"	\
       "movl %eax, (_architecture + 18)				\n"	\
									\
       /*								\
	* 4)								\
	*/								\
									\
       "movl 36(%esp), %ebx					\n"	\
									\
       /*								\
	* 5)								\
	*/								\
									\
       "movl (_architecture + 2), %eax				\n"	\
       "cmp %eax, (_architecture + 18)				\n"	\
       "je 1f							\n"	\
       "movl %eax, %cr3						\n"	\
       "1:							\n"	\
									\
       /*								\
	* 6)								\
	*/								\
									\
       "movw (_architecture + 0), %ax				\n"	\
       "movw %ax, %ds						\n"	\
       "movw %ax, %es						\n"	\
       "movw %ax, %fs						\n"	\
       "movw %ax, %gs						\n"	\
									\
       /*								\
	* 7)								\
	*/								\
									\
       "movl %esp, (_architecture + 22)				\n"	\
									\
       /*								\
	* 8)								\
	*/								\
									\
       "movl (_architecture + 14), %esp				");

/*
 * this macro-function restores the context of the thread whose PDBR and pile
 * are referenced in _architecture.thread. as such, the whole ia32/educational
 * context switch mechanism relies on the simple fact that changing
 * the _architecture structure and returning from the interrupt will make
 * the thread's context restored and its execution resumed.
 *
 * note that at this point, the environment is composed of the kernel PDBR
 * and the KIS - Kernel Interrupt Stack.
 *
 * steps:
 *
 * 1) if the current PDBR i.e CR3 is different from the PDBR of the thread
 *    to restore (_architecture.thread.pdbr), update CR3 with
 *    _architecture.thread.pdbr. otherwise do nothing.
 * 2) retrieve the thread's pile pointer (_architecture.thread.pile.esp)
 *    and update ESP with it. by doing this, the system now operates on
 *    the thread's pile i.e no longer using the KIS - Kernel Interrupt Stack.
 * 3) retrieve the DS - Data Segment selector from the pile: it has been
 *    push by ARCHITECTURE_CONTEXT_SAVE(). then, update the segment selectors
 *    DS, ES, FS and GS.
 * 4) retrieve the value of the general-purpose registers from the stack.
 */

#define ARCHITECTURE_CONTEXT_RESTORE()					\
  asm (									\
       /*								\
	* 1)								\
	*/								\
									\
       "movl %cr3, %ebx						\n"	\
       "movl (_architecture + 18), %eax				\n"	\
       "cmp %eax, %ebx						\n"	\
       "je 1f							\n"	\
       "movl %eax, %cr3						\n"	\
       "1:							\n"	\
									\
       /*								\
	* 2)								\
	*/								\
									\
       "movl (_architecture + 22), %edx				\n"	\
       "movl %edx, %esp						\n"	\
									\
       /*								\
	* 3)								\
	*/								\
									\
       "popl %eax						\n"	\
       "movw %ax, %ds						\n"	\
       "movw %ax, %es						\n"	\
       "movw %ax, %fs						\n"	\
       "movw %ax, %gs						\n"	\
									\
       /*								\
	* 4)								\
	*/								\
									\
       "popa							");

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this structure represents the IA32 context.
 */

typedef struct
{
  t_reg32	ds;
  t_reg32	edi;
  t_reg32	esi;
  t_reg32	ebp;
  t_reg32	_esp;
  t_reg32	ebx;
  t_reg32	edx;
  t_reg32	ecx;
  t_reg32	eax;
  t_reg32	error;
  t_reg32	eip;
  t_reg32	cs;
  t_reg32	eflags;
  t_reg32	esp;
  t_reg32	ss;
}		__attribute__ ((packed)) as_context;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *	../context.c
 */

/*
 * ../context.c
 */

t_status		architecture_context_dump(as_context	context);

t_status		architecture_context_build(i_thread	id);

t_status		architecture_context_destroy(i_thread	id);

t_status		architecture_context_setup(void);

t_status		architecture_context_locate(void);

t_status		architecture_context_switch(i_thread	current,
						    i_thread	future);

t_status		architecture_context_arguments(i_thread	id,
						       void*	arguments,
						       t_vsize	size);

t_status		architecture_context_get(i_thread	id,
						 as_context*	context);

t_status		architecture_context_set(i_thread	id,
						 as_context*	context);


/*
 * eop
 */

#endif
