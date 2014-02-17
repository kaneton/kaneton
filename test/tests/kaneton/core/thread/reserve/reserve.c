/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ts/kaneton/core/thread/reserve/reserve.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:09:45 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "reserve.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel		_kernel;

void			test_core_thread_reserve_content(void)
{
  TEST_HANG("unreachable");
}

void			test_core_thread_reserve(void)
{
  i_thread		thread;

  TEST_ENTER();

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_thread_reserve_content,
		     &thread) != STATUS_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread) != STATUS_OK)
    TEST_ERROR("[thread_start] error");

  TEST_SIGNATURE(r2390fiw390ggh4r);

  TEST_LEAVE();
}
