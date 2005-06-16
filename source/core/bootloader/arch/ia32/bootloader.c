/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bootloader.c
 * 
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Thu Jun 16 15:55:52 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

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
 * 1) initializes the console and checks the magic number.
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

int			bootloader(t_uint32			magic,
				   multiboot_info_t*		mbi)
{

  /*
   * 1)
   */

  cons_init();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    bootloader_error();

  printf("\n                      --- the kaneton operating system ---\n\n");

  /*
   * 2)
   */

  kernel = (void (*)(t_init*))init_relocate(mbi);

  /*
   * 3)
   */

  pmode_init();

  /*
   * 4)
   */

  paging_init();

  /*
   * 5)
   */

  init_segments();
  init_regions();

  /*
   * 6)
   */

#if (IA32_DEBUG & IA32_DEBUG_INIT)
  init_dump();
#endif

  /*
   * 7)
   */

  cons_load();

  /*
   * 8)
   */

  asm volatile ("movl %%ebp, %0\n"
		"movl %%esp, %1\n"
		: "=m" (ebp), "=m" (esp)
		:);

  asm volatile ("movl %0, %%ebp\n"
		"movl %0, %%esp\n"
		"pushl %1\n"
		:
		: "g" (init->kstack + init->kstacksz - 1),
		  "g" (init));

  /*
   * 9)
   */

  while (1);

  kernel(init);

  asm volatile ("movl %0, %%ebp\n"
		"movl %1, %%esp\n"
		:
		: "g" (ebp), "g" (esp));

  /*
   * 10)
   */

  cons_msg('!', "error: kernel exited\n");
  bootloader_error();
}

/*
 * XXX
 *
 * - gerer de grands noms de modules
 */
