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
 * updated       julien quintard   [mon feb  7 16:19:17 2011]
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
 */

#define	ARCHITECTURE_CONTEXT_SAVE()					\
  /* FIXME[code to complete] */

/*
 * this macro-function restores the context of the thread whose PDBR and pile
 * are referenced in _architecture.thread. as such, the whole ia32/educational
 * context switch mechanism relies on the simple fact that changing
 * the _architecture structure and returning from the interrupt will make
 * the thread's context restored and its execution resumed.
 *
 * note that at this point, the environment is composed of the kernel PDBR
 * and the KIS - Kernel Interrupt Stack.
 */

#define ARCHITECTURE_CONTEXT_RESTORE()					\
  /* FIXME[code to complete] */

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

t_status			architecture_context_dump(as_context	context);

t_status			architecture_context_build(i_thread	id);

t_status			architecture_context_destroy(i_thread	id);

t_status			architecture_context_setup(void);

t_status			architecture_context_locate(void);

t_status			architecture_context_switch(i_thread	current,
						    i_thread	future);

t_status			architecture_context_arguments(i_thread	id,
						       void*	arguments,
						       t_vsize	size);

t_status			architecture_context_get(i_thread	id,
						 as_context*	context);

t_status			architecture_context_set(i_thread	id,
						 as_context*	context);


/*
 * eop
 */

#endif
