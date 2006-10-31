/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-octane.mips64/bootloader.c
 *
 * created       enguerrand raymond   [wed oct 18 14:21:40 2006]
 * updated       enguerrand raymond   [sun oct 22 21:57:17 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */


/*
 * the init variable.
 */

extern	t_init		init;

/*
 * these three variable are globals to avoid them to be lost when the
 * bootloader changes its stack.
 *
 * indeed, before jumping on the kernel a new stack is created so the local
 * variables will no longer be accessible. putting these variables in
 * globals bypass this problem.
 */

void			(*kernel)(t_init*);

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * a funny function which does nothing.
 *
 * this function is called when a fatal error occurs.
 */

void			bootloader_error(void)
{
  while (1)
    ;
}

/*
 * the bootloader entry point.
 *
 * steps:
 *
 * 1) initialise the console.
 */

int			bootloader(void)
{

  /*
   * 1)
   */

  bootloader_cons_init();

  printf("\n");
  printf("                --- the kaneton microkernel project ---\n");
  printf("\n");

  /*
   * 2)
   */

  bootloader_cons_msg('!', "error: kernel exited\n");
  //bootloader_error();

  return (0);
}
