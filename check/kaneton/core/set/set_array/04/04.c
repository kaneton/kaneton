/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_array/04/04.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:21:59 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests the ORGANISE option.
 */

void		check_set_array_04(void)
{
  i_set	id;

  TEST_ENTER();

  if (set_reserve(array, SET_OPT_ORGANISE | SET_OPT_ALLOC, 4,
		  sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  check_many_add(id, 4, 80LL, 98654LL, 42LL, 122LL);

  check_many_remove(id, 1, 42LL);

  check_many_add(id, 2, 45LL, 64LL);

  check_display_set(id, NULL);

  check_many_add(id, 4, 987LL, 368LL, 2LL, 123LL);

  check_many_remove(id, 2, 368LL, 80LL);

  check_many_add(id, 1, 452LL);

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE();
}
