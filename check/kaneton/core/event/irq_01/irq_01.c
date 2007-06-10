/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:41:04 2006]
 */

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int	thrown = 0;

static void	check_irq6(t_id	id)
{
  printf(" + IRQ triggered\n");
  thrown++;
}

/*
 * reserve event onto int3 and throw an int3.
 */

void		check_event_irq_01(void)
{
  int		i;

  TEST_ENTER();

  ASSERT(event_reserve(32 + 6,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_irq6)) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Triggering IRQ\n");
  OUTB(0x3F0 + 2, 0);
  for (i = 0; i < 10000; i++)
    asm volatile("nop");
  OUTB(0x3F0 + 2, 0x0C);
  for (i = 0; i < 10000000; i++)
    asm volatile("nop");
  OUTB(0x3F0 + 2, 0);
  for (i = 0; i < 10000; i++)
    asm volatile("nop");
  OUTB(0x3F0 + 2, 0x0C);
  for (i = 0; i < 10000000; i++)
    asm volatile("nop");
  ASSERT(thrown == 2, " x One or more IRQ was not caught\n");
  printf(" - Execution resumed\n");

  TEST_LEAVE();
}
