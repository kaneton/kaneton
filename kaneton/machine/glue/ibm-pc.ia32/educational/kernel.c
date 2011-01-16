/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/glue/ibm-pc.ia32/educational/kernel.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [sat jan 15 06:10:17 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the kernel manager's glue.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the kernel dispatcher.
 */

d_kernel		glue_kernel_dispatch =
  {
    NULL,
    NULL,
    NULL
  };
