/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ia32/educational/event/exception/07/07.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2070]
 * updated       julien quintard   [fri dec 17 14:52:00 2010]
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
static volatile t_vaddr		address = 0;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_architecture_event_exception_07_handler(t_id id,
								     t_vaddr data)
{
  i_segment		segid;
  i_region		regid;
  i_thread		i;
  o_thread*		o;

  if (thread_current(&i) != ERROR_OK)
    TEST_ERROR("[thread_current] error");

  if (thread_get(i, &o) != ERROR_OK)
    TEST_ERROR("[thread_get] error");

  thrown = 1;

  if (!((!(o->machine.error & (1 << 0)) &&	/* non present page */
	 (o->machine.error & (1 << 1)) &&	/* write access */
	 !(o->machine.error & (1 << 2)) &&	/* supervisor mode when exception */
	 !(o->machine.error & (1 << 3)) &&	/* reserved bit error */
	 !(o->machine.error & (1 << 4)))))
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
                     address,
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
  o_region*		reg;

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

  if (region_get(_kernel->as, regids[9], &reg) != ERROR_OK)
    TEST_ERROR("[region_get] error");

  address = reg->address;

  for (i = 0; i < 10; i++)
    if (region_release(_kernel->as, regids[i]) != ERROR_OK)
      TEST_ERROR("[region_release] error");

  if (segment_release(segid) != ERROR_OK)
    TEST_ERROR("[segment_release] error");

  event_release(14);

  if (event_reserve(14,
		    EVENT_TYPE_FUNCTION,
		    EVENT_ROUTINE(test_architecture_event_exception_07_handler),
		    EVENT_DATA(NULL)) != ERROR_OK)
    TEST_ERROR("[event_reserve] error");

  ptr = (t_uint8*)address + 0x42;

  *ptr = 0x42;

  asm volatile("nop");

  if (thrown != 1)
    TEST_ERROR("the exception has not been caught");

  if (event_release(14) != ERROR_OK)
    TEST_ERROR("[event_release] error");

  TEST_SIGNATURE(0i3rfw90ug8938hy3);

  TEST_LEAVE();
}
