/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/enguerrand/...oot/bootloader/qemu-mips.mips64/opmode.c
 *
 * created       enguerrand raymond   [mon mar 16 23:13:41 2009]
 * updated       enguerrand raymond   [sat apr 11 02:38:42 2009]
 */

/*
 * this file provides functions to configure the microprocessor
 * to 64 bits mode for kernel, supervisor and user memory space
 * and to switch the microprocessor to normal mode
 */

/*
 * ---------- dependencies --------------------------------------------------------
 */

#include "libmips64.h"

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * switch the microprocessor to error mode by setting
 * the ERL bit of the status register
 */

void	enable_error_mode(void)
{
  set_status_register(ERL_SET);
}

/*
 * activate the normal mode by clearing the 
 * ERL bit of the status register
 */

void	disable_error_mode(void)
{
  clear_status_register(ERL_CLEAR);
}

/*
 * switch the microprocessor to exception mode
 * by setting the EXL bit of the status register
 */

void	enable_exception_mode(void)
{
  set_status_register(EXL_SET);
}

void	disable_interrupt(void)
{
  clear_status_register(IE_CLEAR);
}

/*
 * switch the microprocessor to the normal mode by
 * clearing the BEV bit in the DS field of the status
 * register
 */

void	disable_bootstrap_mode(void)
{
  clear_status_register(BEV_CLEAR);
}

/*
 * set the real kernel mode with by clearing
 * the KSU bits of the status registers
 */

void	set_kernel_mode(void)
{
  clear_status_register(KSU_CLEAR);
}
