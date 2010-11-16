/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/event/ia32/exception/07/07.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2070]
 * updated       julien quintard   [wed nov 17 00:03:10 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "07.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern i_as			kasid;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		thrown = 0;
static volatile t_paddr		region = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_event_ia32_exception_07_handler(t_id	id,
							     t_vaddr	address,
							     t_uint32	error)
{
  i_segment		segid;
  i_region		regid;

  thrown = 1;

  if (!((!(error & (1 << 0)) &&		/* non present page */
	 (error & (1 << 1)) &&		/* write access */
	 !(error & (1 << 2)) &&		/* supervisor mode when exception occured */
	 !(error & (1 << 3)) &&		/* reserved bit error */
	 !(error & (1 << 4)))))
    TEST_ERROR("invalid error code\n");

  if (segment_reserve(kasid,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &segid) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(kasid,
		     segid,
		     0,
		     REGION_OPT_FORCE,
                     region,
		     PAGESZ,
		     &regid) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");
}

void			test_event_ia32_exception_07(void)
{
  t_uint8*		ptr;
  i_segment		segid;
  i_region		regids[10];
  t_uint32		i;

  TEST_ENTER();

  if (segment_reserve(kasid,
		      PAGESZ,
		      PERM_READ | PERM_WRITE,
		      &segid) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  for (i = 0; i < 10; i++)
    if (region_reserve(kasid,
		       segid,
		       0,
		       REGION_OPT_NONE,
		       0,
		       PAGESZ,
		       &regids[i]) != ERROR_NONE)
      TEST_ERROR("[region_reserve] error\n");

  region = (t_paddr)regids[9];

  for (i = 0; i < 10; i++)
    if (region_release(kasid, regids[i]) != ERROR_NONE)
      TEST_ERROR("[region_release] error\n");

  if (segment_release(segid) != ERROR_NONE)
    TEST_ERROR("[segment_release] error\n");

  event_release(14);

  if (event_reserve(14,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_event_ia32_exception_07_handler),
		    0) != ERROR_NONE)
    TEST_ERROR("[event_reserve] error\n");

  ptr = (t_uint8*)region + 0x42;

  *ptr = 0x42;

  asm volatile("nop");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught\n");

  TEST_LEAVE();
}
