/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...rchitecture/ia32/event/exception/07/07.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2070]
 * updated       julien quintard   [sat dec  4 12:23:59 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "07.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel*		_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile int		thrown = 0;
static volatile t_paddr		region = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_exception_07_handler(t_id id,
								     t_vaddr address,
								     t_uint32 error)
{
  i_segment		segid;
  i_region		regid;

  thrown = 1;

  if (!((!(error & (1 << 0)) &&		/* non present page */
	 (error & (1 << 1)) &&		/* write access */
	 !(error & (1 << 2)) &&		/* supervisor mode when exception */
	 !(error & (1 << 3)) &&		/* reserved bit error */
	 !(error & (1 << 4)))))
    TEST_ERROR("invalid error code");

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &segid) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  if (region_reserve(_kernel->as,
		     segid,
		     0,
		     REGION_OPTION_FORCE,
                     region,
		     PAGESZ,
		     &regid) != ERROR_OK)
    TEST_ERROR("[region_reserve] error");
}

void			test_architecture_event_exception_07(void)
{
  t_uint8*		ptr;
  i_segment		segid;
  i_region		regids[10];
  t_uint32		i;

  TEST_ENTER();

  if (segment_reserve(_kernel->as,
		      PAGESZ,
		      PERMISSION_READ | PERMISSION_WRITE,
		      &segid) != ERROR_OK)
    TEST_ERROR("[segment_reserve] error");

  for (i = 0; i < 10; i++)
    if (region_reserve(_kernel->as,
		       segid,
		       0,
		       REGION_OPTION_NONE,
		       0,
		       PAGESZ,
		       &regids[i]) != ERROR_OK)
      TEST_ERROR("[region_reserve] error");

  region = (t_paddr)regids[9];

  for (i = 0; i < 10; i++)
    if (region_release(_kernel->as, regids[i]) != ERROR_OK)
      TEST_ERROR("[region_release] error");

  if (segment_release(segid) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  event_release(14);

  if (event_reserve(14,
		    EVENT_FUNCTION,
		    EVENT_HANDLER(test_architecture_event_exception_07_handler),
		    0) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  ptr = (t_uint8*)region + 0x42;

  *ptr = 0x42;

  asm volatile("nop");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught");

  if (event_release(14) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(0i3rfw90ug8938hy3);

  TEST_LEAVE();
}
