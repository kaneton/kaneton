/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/export_kaneton/kaneton/core/bootloader/arch/ia32-segment/bootloader.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [mon jan 30 20:16:18 2006]
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * the goal of the bootloader is to install a correct environment for
 * the kernel.
 *
 * the first thing to do is to relocate the different kaneton data structure
 * in order to build the t_init structure. this structure must contain
 * everything the kernel will need.
 *
 * for more information on the relocating, take a look to the kaneton
 * paper which describes the entire kaneton reference.
 *
 * once the init structure was built, the kernel has to be launched.
 *
 * nevertheless, the  kernel needs to  evolve with the  protected mode
 * activated, so the bootloader first has to install these.
 *
 * printing some messages is interesting showing the protected mode is
 * correctly installed.
 *
 * after all, the bootloader has launch the kernel binary.
 *
 * look at the ld scripts to know where the kernel has to be loaded.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * these three variable are globals to avoid them to be lost when the
 * bootloader changes its stack.
 *
 * indeed, before jumping on the kernel a new stack is created so the local
 * variables will no longer be accessible. putting these variables in
 * globals bypass this problem.
 */

void			(*kernel)(t_init*);

t_reg32			ebp;
t_reg32			esp;

/*
 * ---------- functions -------------------------------------------------------
 */

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi)
{

/*                                                                  [cut] k1 */

/*                                                                 [cut] /k1 */

  return (0);
}
