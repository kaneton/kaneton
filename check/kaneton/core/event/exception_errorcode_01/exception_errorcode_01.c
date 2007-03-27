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

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

extern i_as		kasid;

static volatile int	thrown = 0;

static void	check_pf(t_id	id, t_uint32 error_code)
{
  i_segment	segid;
  i_region	regid;

  printf(" + Exception thrown\n");
  thrown = 1;
  printf(" + Checking error code\n");
  if (!(error_code & (1 << 0)) // non present page
      && (error_code & (1 << 1)) // write access
      && !(error_code & (1 << 2)) // supervisor mode when exception occured
      && !(error_code & (1 << 3)) // reserved bit error
      && !(error_code & (1 << 4)))
    {
      // ok
    }
  else
    printf(" x Bad error code\n");

  if (segment_reserve(kasid, PAGESZ, PERM_READ | PERM_WRITE, &segid) !=
      ERROR_NONE)
    printf("Cannot segment_reserve\n");

  if (region_reserve(kasid, segid, 0, REGION_OPT_FORCE,
		     MK_BASE(0x8abcdef0), PAGESZ, &regid) != ERROR_NONE)
    printf("Cannot region_reserve\n");
}

/*
 * reserve event onto int3 and throw an int3.
 */

void		check_event_exception_errorcode_01(void)
{
  t_uint8*	ptr = (t_uint8*)0x8abcdef0;

  TEST_ENTER();

  event_release(14);

  ASSERT(event_reserve(14,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_pf)) == ERROR_NONE,
	 "cannot event_reserve\n");

  printf(" - Throwing exception\n");
  *ptr = 0x42;
  asm volatile("nop");
  ASSERT(thrown == 1, " x Exception not caught\n");
  printf(" - Execution resumed\n");

  TEST_LEAVE();
}
