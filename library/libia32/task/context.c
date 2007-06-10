/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/task/context.c
 *
 * created       renaud voltz   [tue apr  4 21:45:07 2006]
 * updated       matthieu bucchianeri   [tue oct 10 21:59:41 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file contains some useful functions related to contexts.
 *
 * the ia32_context structure is defined in the header and contains
 * all the processor state.
 *
 * the function context_setup is mainly used to detect processor's
 * extended registers such as MMX or SSE registers.
 *
 * the function context_copy is used to copy from a context only the
 * significant fields of a context structure.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * stores the interrupted context.
 */

t_ia32_context*	context = NULL;

/*
 * tells the processor capabilities.
 */

t_uint32	cpucaps = 0;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * initialize a few things about contexts.
 *
 * steps:
 *
 * 1) initialize the FPU.
 * 2) detect MMX and SSE processor capabilities.
 */

void			context_setup(void)
{
  /*
   * 1)
   */

  FINIT();

  /*
   * 2)
   */

  if (cpuid_has_mmx())
    cpucaps |= IA32_CAPS_MMX;

  if (cpuid_has_sse() && cpuid_has_fxstate())
    {
      extensions_enable_sse();
      cpucaps |= IA32_CAPS_SSE;
    }
}

/*
 * this function copies a context. useful for context switching.
 */

void			context_copy(t_ia32_context*			dst,
				     const t_ia32_context*		src)
{
  dst->eax = src->eax;
  dst->ebx = src->ebx;
  dst->ecx = src->ecx;
  dst->edx = src->edx;
  dst->esi = src->esi;
  dst->edi = src->edi;
  dst->esp = src->esp;
  dst->ebp = src->ebp;
  dst->eip = src->eip;
  dst->cr3 = src->cr3;
  dst->cs = src->cs;
  dst->ds = src->ds;
  dst->es = src->es;
  dst->fs = src->fs;
  dst->gs = src->gs;
  dst->ss = src->ss;
  dst->eflags = src->eflags;
}
