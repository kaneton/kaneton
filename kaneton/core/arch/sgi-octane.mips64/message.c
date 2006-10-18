/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-octane.mips64/message.c
 *
 * created       enguerrand raymond   [wed oct 18 14:26:39 2006]
 * updated       enguerrand raymond   [wed oct 18 14:26:44 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the message manager dispatch.
 */

d_message		message_dispatch =
  {
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */
