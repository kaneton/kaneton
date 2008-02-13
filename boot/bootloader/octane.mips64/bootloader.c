/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-o2.mips64/bootloader.c
 *
 * created       enguerrand raymond   [wed oct 18 14:21:40 2006]
 * updated       enguerrand raymond   [sun jan 28 00:52:39 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <libc.h>
#include <kaneton.h>

#include "bootloader.h"
#include "init.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern	t_init*		init;

/*
 * these three variable are globals to avoid them to be lost when the
 * bootloader changes its stack.
 *
 * indeed, before jumping on the kernel a new stack is created so the local
 * variables will no longer be accessible. putting these variables in
 * globals bypass this problem.
 */

void			(*kernel)(t_init*) = KERNEL_BASE64;

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
 * 1) initializes the console and displays welcome messages
 * 2) Init t_init variable
 * 3) Fill the t_init fields
 * 4) Initialize the o_segment array with kernel segment
 * 5) Initialize the kernel stack
 * 6) Setting up the 64 bits mode for the 64 bits kernel
 * 7) Jump to the kernel
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

  init = (t_init*)bootloader_alloc(PAGESZ);

  /*
   * 3)
   */

  bootloader_cons_msg('+', "Init structure setting up\n");

  bootloader_init();

  /*
   * 4)
   */

  bootloader_cons_msg('+', "Segment array setting up\n");

  bootloader_segments_init();

  /*
   * 5) 6) 7)
   */

  bootloader_cons_msg('+', "Kernel stack setting up\n");

  bootloader_cons_msg('+', "64 bits mode setting up\n");

  printf("\n");
  bootloader_cons_msg('+', "Jump to the kernel\n");

  asm("move $10, $0\n\t"
      "lw $9, %0\n\t"
      "lw $10, %1\n\t"
      "addu $10, $10, $9\n\t"
      //      "addiu $10, $10, -4\n\t"
      "move $29, $10\n\t"
      "move $30, $29\n\t"
      :
      : "g" (init->kstack),"g" (init->kstacksz)
      : "$10", "$9"
      );

  asm("li $10, 0x300000e0\n\t"
      "mtc0 $10, $12\n\t");

  kernel(init);

  bootloader_cons_msg('!', "error: kernel exited\n");

  bootloader_error();

  return (0);
}
