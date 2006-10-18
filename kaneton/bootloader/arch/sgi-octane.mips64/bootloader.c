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
 * updated       enguerrand raymond   [wed oct 18 14:21:48 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>
#include <libmipsr10000.h>
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
 * TODO: Write Commentaries about bootloader function works.
 */
int		bootloader(t_uint32			magic)
{
  while (1)
    ;
  return 0;
}
