/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-octane.mips64/cpu.c
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:37:40 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 22:50:54 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
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
 * the cpu manager dispatch.
 */

d_cpu		cpu_dispatch =
  {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

