/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/task/task.c
 *
 * created       renaud voltz   [tue apr  4 21:45:07 2006]
 * updated       matthieu bucchianeri   [wed jul 26 16:34:52 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX THREAD information need to be written.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
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

void			task_setup(void)
{
  FINIT();

  if (cpuid_has_mmx())
    cpucaps |= IA32_CAPS_MMX;


  if (cpuid_has_sse() && cpuid_has_fxstate())
    {
      extensions_enable_sse();
      cpucaps |= IA32_CAPS_SSE;
    }
}

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
