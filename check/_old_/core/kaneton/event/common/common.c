/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/event/common/common.c
 *
 * created       renaud voltz   [sun mar 19 00:56:51 2006]
 * updated       renaud voltz   [sun mar 19 00:56:51 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "common.h"

void	check_event_common(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
}

/*
 * starts ALL tests related to event manager
 */

void	check_event_tests(void)
{
  check_event_common();
  check_event_01();
  check_event_02();
  check_event_03();
  /* XXX continue tests */
}

/*
 * fake event handler
 */

void	fake_event_handler(t_uint32				id)
{
  printf("event %u was detected !/n", id);
}
