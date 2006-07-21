/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/event/02/02.c
 *
 * created       renaud voltz   [sun mar 19 17:16:40 2006]
 * updated       renaud voltz   [sun mar 19 17:16:40 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * check event_resere() bad arguments.
 *
 * steps:
 *
 * 1) event id out of range.
 * 2) reserve several times the same event without releasing it.
 */

void                    check_event_02(void)
{
  TEST_ENTER;

  /*
   * 1)
   */

  if (event_reserve(2000, EVENT_FUNCTION, (u_event_handler)fake_event_handler)
      == ERROR_NONE)
    printf("error: event_reserve() accepted a bad event id.\n");

  /*
   * 2)
   */

  if (event_reserve(10, EVENT_MESSAGE, (u_event_handler)(t_tskid)42)
      != ERROR_NONE)
    printf("error: event_reserve()\n");

  if (event_reserve(10, EVENT_FUNCTION, (u_event_handler)fake_event_handler)
      == ERROR_NONE)
    printf("error: event_reserve() reserved twice the same event !\n");

  if (event_release(10) != ERROR_NONE)
    printf("error: event_release()\n");

  TEST_LEAVE;
}
