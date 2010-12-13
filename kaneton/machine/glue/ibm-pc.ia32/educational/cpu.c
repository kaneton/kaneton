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
 * updated       julien quintard   [thu dec  9 21:40:52 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the file provides the machine-specific CPU functions.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the init structure.
 */

extern s_init*		_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the cpu manager dispatch.
 */

d_cpu			glue_cpu_dispatch =
  {
    NULL,
    NULL,
    glue_cpu_current,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function returns the current CPU.
 *
 * since the ia32/educational implementation assumes a single processor,
 * the current CPU remains the BSP, the only processor.
 */

t_error			glue_cpu_current(i_cpu*			cpu)
{
  *cpu = _init->bsp;

  MACHINE_LEAVE();
}
