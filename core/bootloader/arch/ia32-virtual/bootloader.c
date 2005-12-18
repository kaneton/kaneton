/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.c
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Mon Nov 14 21:48:03 2005   mycure
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
 * nevertheless, the kernel needs to evolve with the protected mode and
 * paging mode activated, so the bootloader first has to install these.
 *
 * printing some messages is interesting showing the protected mode and
 * paging mode are correctly installed.
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

/*                                                                  [cut] k1 */

/*
 * a funny function which do nothing.
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
 * 1) initialises the console and checks the magic number.
 * 2) relocates binaries, data, stack
 * 3) installs the protected mode.
 * 4) installs the paging mode.
 * 5) computes the segments and regions to pass to the kernel.
 * 6) dumps the init structure if required.
 * 7) loads the console state for the kernel.
 * 8) update registers for the new kernel stack.
 * 9) then, the kernel is launched.
 * 10) this part is only reached if the kernel exit.
 */

/*                                                                 [cut] /k1 */

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi)
{

/*                                                                  [cut] k1 */

  /*
   * 1)
   */

  bootloader_cons_init();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    bootloader_error();

  printf("\n");
  printf("                --- the kaneton microkernel project ---\n");
  printf("\n");

  /*
   * 2)
   */

  kernel = (void (*)(t_init*))bootloader_init_relocate(mbi);

  /*
   * 3)
   */

  bootloader_pmode_init();

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

  bootloader_cons_load();

  /*
   * 8)
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
      : "g" (init->kstack + init->kstacksz - 1),
        "g" (init)
     );

  /*
   * 9)
   */

  kernel(init);

  asm("movl %0, %%ebp\n\t"
      "movl %1, %%esp\n\t"
      :
      : "g" (ebp), "g" (esp)
     );

  /*
   * 10)
   */

  bootloader_cons_msg('!', "error: kernel exited\n");
  bootloader_error();

/*                                                                 [cut] /k1 */

  return (0);
}
