/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ne/glue/ibm-pc.ia32/educational/region.c
 *
 * created       julien quintard   [wed dec 14 07:06:44 2005]
 * updated       julien quintard   [sat feb  5 14:37:43 2011]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the region manager's glue which basically consists
 * in updating the address space's page directory, tables etc. in order
 * to reflect the core operation.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region dispatcher.
 */

d_region		glue_region_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  };
