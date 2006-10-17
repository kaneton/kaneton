/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-octane.mips64/message.c
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:38:52 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 22:42:07 2006]
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
