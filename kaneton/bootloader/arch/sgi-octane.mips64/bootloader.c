/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-octane.mips64/bootloader.c
 *
 * created       Enguerrand RAYMOND   [sun oct 15 20:46:15 2006]
 * updated       Enguerrand RAYMOND   [sun oct 15 20:52:46 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>
#include <liboctane.h>

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
  return 0;
}
