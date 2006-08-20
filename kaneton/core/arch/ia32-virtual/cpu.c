/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:01 2006]
 * updated       matthieu bucchianeri   [fri aug 18 19:55:47 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_cpu*	cpu;

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
 * this function get the current cpu id. always 0 since mono-processor mode.
 */

t_error			cpu_current(i_cpu*			cpuid)
{
  *cpuid = 0;
}
