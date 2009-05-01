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

#include <libc/libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile unsigned short	gs = (16 << 3);

static volatile int	thrown = 0;

static void	check_np(t_id	id, t_vaddr pv, t_uint32 error_code)
{
  printf(" + Exception thrown\n");
  thrown = 1;
  gs = 0x10;
}

/*
 * reserve event onto
 */

void		check_event_exception_errorcode_01(void)
{
  TEST_ENTER();

  ia32_gdt_delete_segment(NULL, 16);

  assert(event_reserve(13,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_np), 0) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Throwing exception\n");
  asm volatile("movw %0, %%gs"
	       :
	       : "m" (gs));
  assert(thrown == 1, " x Exception not caught\n");
  printf(" - Execution resumed\n");

  TEST_LEAVE();
}
