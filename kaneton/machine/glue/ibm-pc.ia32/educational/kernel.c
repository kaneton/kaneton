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
 * updated       julien quintard   [sun dec 19 17:33:50 2010]
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

#include <glue/glue.h>
#include <architecture/architecture.h>
#include <platform/platform.h>

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
