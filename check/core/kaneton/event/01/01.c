/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/event/01/01.c
 *
 * created       renaud voltz   [sun mar 19 16:26:50 2006]
 * updated       renaud voltz   [sun mar 19 16:26:50 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * reserve and release events.
 *
 * steps:
 *
 * 1) reserve an event and release it.
 * 2) reserve and release 10 times the same event.
 * 3) reserve several events and then release it.
 */

void			check_event_01(void)
{
  t_uint32		i;

  TEST_ENTER;

  /*
   * 1)
   */

  if (event_reserve(0, EVENT_FUNCTION, (u_event_handler)fake_event_handler)
      != ERROR_NONE)
    printf("error: event_reserve()\n");

  if (event_release(0) != ERROR_NONE)
    printf("error: event_release()\n");

  /*
   * 2)
   */

  for (i = 0; i < 10; i++)
    {
      if (event_reserve(1, EVENT_FUNCTION, (u_event_handler)fake_event_handler)
	  != ERROR_NONE)
	printf("error: event_reserve()\n");

      if (event_release(1) != ERROR_NONE)
	printf("error: event_release()\n");
    }

  /*
   * 3)
   */

  if (event_reserve(2, EVENT_MESSAGE, (u_event_handler)(t_tskid)42)
      != ERROR_NONE)
    printf("error: event_reserve()\n");

  if (event_reserve(26, EVENT_MESSAGE, (u_event_handler)(t_tskid)42)
      != ERROR_NONE)
    printf("error: event_reserve()\n");

  if (event_reserve(15, EVENT_FUNCTION, (u_event_handler)fake_event_handler)
      != ERROR_NONE)
    printf("error: event_reserve()\n");

  if (event_release(26) != ERROR_NONE)
    printf("error: event_release()\n");

  if (event_release(15) != ERROR_NONE)
    printf("error: event_release()\n");

  if (event_release(2) != ERROR_NONE)
    printf("error: event_release()\n");

  TEST_LEAVE;
}
