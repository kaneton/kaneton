/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/event/03/03.c
 *
 * created       renaud voltz   [sun mar 19 17:31:23 2006]
 * updated       renaud voltz   [sun mar 19 17:31:23 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * check event_release() bad arguments.
 *
 * steps:
 *
 * 1) event id out of range.
 * 2) release an event which has not been reserved before.
 * 3) release an event which has already been released.
 */

void                    check_event_03(void)
{
  TEST_ENTER;

  /*
   * 1)
   */

  if (event_release(1000) == ERROR_NONE)
    printf("error: event_release() released an non existing event\n");

  /*
   * 2)
   */

  if (event_release(3) == ERROR_NONE)
    printf("error: event_release() released an non reserved event\n");

  /*
   * 3)
   */

  if (event_reserve(5, EVENT_FUNCTION, (u_event_handler)fake_event_handler)
      != ERROR_NONE)
    printf("error: event_reserve()\n");

  if (event_release(5) != ERROR_NONE)
    printf("error: event_release()\n");

  if (event_release(5) == ERROR_NONE)
    printf("error: event_release() released an already released event\n");

  TEST_LEAVE;
}
