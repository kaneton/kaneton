/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/arch/ia32-virtual/cpu.c
 *
 * created       matthieu bucchianeri   [sat jul 29 18:04:01 2006]
 * updated       matthieu bucchianeri   [sat jul 29 18:04:23 2006]
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
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

