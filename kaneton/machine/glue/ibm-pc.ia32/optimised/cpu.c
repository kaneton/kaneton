/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kan...machine/glue/ibm-pc.ia32/optimised/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:01 2006]
 * updated       matthieu bucchianeri   [mon dec 24 19:37:13 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc/libc.h>
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
    glue_cpu_current,
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

t_error			glue_cpu_current(i_cpu*			cpuid)
{
  CPU_ENTER(cpu);

  *cpuid = ia32_apic_id();

  CPU_LEAVE(cpu, ERROR_NONE);
}
