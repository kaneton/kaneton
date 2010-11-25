/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chine/glue/ibm-pc.ia32/educational/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:01 2006]
 * updated       julien quintard   [wed nov 24 14:19:45 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the cpu manager.
 */

extern m_cpu*		_cpu;

/*
 * the init structure.
 */

extern t_init*		_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the cpu manager dispatch.
 */

d_cpu			glue_cpu_dispatch =
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
 * identifies the  current cpu. for  non-mp, the current cpu  is the
 * boot processor.
 */

t_error			glue_cpu_current(i_cpu*			cpuid)
{
  CPU_ENTER(_cpu);

  *cpuid = _init->bsp;

  CPU_LEAVE(_cpu, ERROR_OK);
}
