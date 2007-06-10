/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/core/arch/ia32-virtual/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:01 2006]
 * updated       julien quintard   [thu aug 31 20:18:01 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_cpu*	cpu;
extern t_init*	init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the cpu manager dispatch.
 */

d_cpu		cpu_dispatch =
  {
    NULL,
    ia32_cpu_current,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * identifies the  current cpu. since  non-mp, the current cpu  is the
 * boot processor.
 */

t_error			ia32_cpu_current(i_cpu*			cpuid)
{
  CPU_ENTER(cpu);

  *cpuid = init->bsp;

  CPU_LEAVE(cpu, ERROR_NONE);
}
