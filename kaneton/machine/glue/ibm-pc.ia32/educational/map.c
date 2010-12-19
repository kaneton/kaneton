/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...chine/glue/ibm-pc.ia32/educational/map.c
 *
 * created       matthieu bucchianeri   [sat jun 16 18:10:38 2007]
 * updated       julien quintard   [sun dec 19 17:33:56 2010]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements the map manager's glue.
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
 * the map dispatcher.
 */

d_map			glue_map_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  };
