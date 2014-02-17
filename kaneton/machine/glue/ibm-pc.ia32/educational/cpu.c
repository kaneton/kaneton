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
 * updated       julien quintard   [sun dec 19 16:42:48 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the file implements the CPU manager's glue.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- externs ---------------------------------------------------------
 */

/*
 * the init structure provided by the boot loader.
 */

extern s_init*		_init;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the CPU dispatcher.
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
 * since the ibm-pc.ia32/educational implementation assumes a single
 * processor, the current CPU is always the BSP - Boot Strap Processor i.e
 * the only processor.
 */

t_status		glue_cpu_current(i_cpu*			cpu)
{
  *cpu = _init->bsp;

  MACHINE_LEAVE();
}
