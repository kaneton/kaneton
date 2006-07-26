/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libia32/task/task.c
 *
 * created       renaud voltz   [tue apr  4 21:45:07 2006]
 * updated       matthieu bucchianeri   [tue jul 25 23:30:03 2006]
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

  if (cpuid_has_sse())
    cpucaps |= IA32_CAPS_SSE;
}
