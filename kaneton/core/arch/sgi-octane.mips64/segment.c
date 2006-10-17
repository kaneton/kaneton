/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-octane.mips64/segment.c
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:40:17 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 22:44:04 2006]
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

#include <liboctane.h>
#include <klibc.h>
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

