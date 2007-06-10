/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-octane.mips64/segment.c
 *
 * created       enguerrand raymond   [wed oct 18 14:27:26 2006]
 * updated       enguerrand raymond   [wed oct 18 14:27:33 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this  file implements dependent  code for  segment manager  on octane
 * with paging architecture.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <libc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment manager dispatch.
 */

d_segment		segment_dispatch =
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
    NULL,
    NULL,
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

