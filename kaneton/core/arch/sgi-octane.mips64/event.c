/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/core/arch/sgi-o2.mips64/event.c
 *
 * created       enguerrand raymond   [wed oct 18 14:26:01 2006]
 * updated       enguerrand raymond   [tue feb 13 00:20:01 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file implements dependent code for the events on octane with paging
 * architecture.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ---------------------------------------------------------
 */

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the event manager dispatch.
 */

d_event				event_dispatch =
  {
    NULL,
    mipsr10k_event_init,
    NULL,
    NULL,
    NULL,
    NULL,
  };

/*
 * ---------- functions -------------------------------------------------------
 */

static void	mipsr10k_event_clock(void)
{
  printf("Clock\n");
}

/*
 * This function set the IE register of the status register
 * to activate all of the eight interrupts and set the interrupt
 * bit in the regisdter IM of the status register
 *
 * 1) Set the IE bit
 * 2) Record interrupt handler in memory
 */

static void		mipsr10k_event_interrupt_activate(void)
{
  unsigned long*	vector_addr = 0xbfc00000;

  /*
   * 1)
   */

  asm("mfc0 $10, $12\n\t"
      "ori $10, $10, 0xff01\n\t"
      "mtc0 $10, $12\n\t");

  /*
   * 2)
   */

  *vector_addr = (unsigned long*)mipsr10k_event_clock;
}

t_error		mipsr10k_event_init(void)
{
  printf("mipsr10k_event_init\n");

  mipsr10k_event_interrupt_activate();

  return ERROR_NONE;
}
