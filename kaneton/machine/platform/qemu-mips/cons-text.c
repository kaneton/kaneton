/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...n/machine/platform/qemu-mips/cons-text.c
 *
 * created       enguerrand raymond   [sun jan 25 14:27:13 2009]
 * updated       enguerrand raymond   [sun jan 25 18:19:44 2009]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements a basic console management for kernel bootup.
 *
 * in the  future, the console control  will be passed  to a dedicated
 * server.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc/libc.h>
#include <kaneton.h>

#include <platform/platform.h>

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function prints a status message.
 *
 * '+' is used for printing information about the execution.
 * '#' is used for printing debug information.
 * '!' is used for printing warning and error messages.
 */

void			cons_msg(char				indicator,
				 char*				fmt,
				 ...)
{

}

t_error			cons_init(void)
{

}
