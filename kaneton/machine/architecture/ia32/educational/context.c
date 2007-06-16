/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/machine/architecture/ia32/educational/context.c
 *
 * created       renaud voltz   [tue apr  4 03:08:03 2006]
 * updated       matthieu bucchianeri   [fri jun 15 09:30:33 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

#include <architecture/architecture.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * stores the interrupted context.
 */

t_ia32_context*	ia32_context = NULL;

/*
 * tells the processor capabilities.
 */

t_uint32	ia32_cpucaps = 0;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function set the pdbr value to all the threads in a task.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) get the destination task.
 * 3) for each thread, update the context's pdbr value.
 */

t_error			ia32_update_pdbr(i_task			tskid,
					 i_as			asid)
{
  o_as*			o;
  o_task*		otask;
  o_thread*		oth;
  t_iterator		it;
  t_state		state;

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  if (task_get(tskid, &otask) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 3)
   */

  set_foreach(SET_OPT_FORWARD, otask->threads, &it, state)
    {
      i_thread*		th;

      if (set_object(otask->threads, it, (void**)&th) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (thread_get(*th, &oth) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (ia32_pd_get_cr3(&oth->machdep.context.cr3, o->machdep.pd,
			  IA32_PD_CACHED, IA32_PD_WRITEBACK) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  return (ERROR_NONE);
}

/*
 * initialize a few things about extended contexts.
 *
 * steps:
 *
 * 1) initialize the FPU.
 * 2) detect MMX and SSE processor capabilities.
 */

void			ia32_extended_context_init(void)
{
  /*
   * 1)
   */

  FINIT();

  /*
   * 2)
   */

  if (ia32_cpu_has_mmx())
    ia32_cpucaps |= IA32_CAPS_MMX;

  if (ia32_cpu_has_sse() && ia32_cpu_has_fxstate())
    {
      ia32_enable_sse();
      ia32_cpucaps |= IA32_CAPS_SSE;
    }
}

/*
 * this function copies a context. useful for context switching.
 */

void			ia32_context_copy(t_ia32_context*		dst,
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
