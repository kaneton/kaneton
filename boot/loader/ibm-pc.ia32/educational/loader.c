/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/bootloader/arch/ibm-pc.ia32-virtual/bootloader.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [tue feb  6 19:16:07 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include "loader.h"

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

t_uint32		ebp;
t_uint32		esp;

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
 * 1) initialize the console and checks the magic number.
 * 2) relocate binaries, data, stack...
 * 3) install the protected mode.
 * 4) install the paging mode.
 * 5) compute the segments and regions to pass to the kernel.
 * 6) dump the init structure if required.
 * 7) update registers for the new kernel stack.
 * 8) then, the kernel is launched.
 * 9) this part is only reached if the kernel exit.
 */

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi)
{
  /*
   * 1)
   */

  bootloader_cons_init();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    bootloader_error();

  bootloader_cons_msg('+', "loader launched\n");

  /*
   * 2)
   */

  kernel = (void (*)(t_init*))bootloader_init_relocate(mbi);

  /*
   * 3)
   */

  bootloader_pmode_init();
  faultcatcher_init();

  /*
   * 4)
   */

  bootloader_paging_init();

  /*
   * 5)
   */

  bootloader_init_segments();
  bootloader_init_regions();

  /*
   * 6)
   */

#if (IA32_DEBUG & IA32_DEBUG_INIT)
  bootloader_init_dump();
#endif

  /*
   * 7)
   */

  asm("movl %%ebp, %0\n\t"
      "movl %%esp, %1\n\t"
      : "=m" (ebp), "=m" (esp)
      :
      : "memory"
     );

  asm("movl %0, %%ebp\n\t"
      "movl %0, %%esp\n\t"
      "pushl %1\n"
      :
      : "g" (init->kstack + init->kstacksz - 128),
        "g" (init)
     );

  /*
   * 8)
   */

  kernel(init);

  asm("movl %0, %%ebp\n\t"
      "movl %1, %%esp\n\t"
      :
      : "g" (ebp), "g" (esp)
     );

  /*
   * 9)
   */

  bootloader_cons_msg('!', "error: kernel exited\n");
  bootloader_error();

  return (0);
}
