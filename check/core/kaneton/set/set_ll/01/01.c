/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_ll/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:22:46 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../init/init.h"

/*
 * creates a linked list set with no options and release it. nothing more.
 */

void		check_set_ll_01(void)
{
  t_setid	id;

  TEST_ENTER;

  if (set_reserve(ll, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  set_release(id);

  if (set_reserve(ll, SET_OPT_ALLOC | SET_OPT_FREE,
		  sizeof(t_id), &id) == ERROR_NONE)
    printf("error: set_reserve() accepted bad arguments\n");

  if (set_reserve(ll, SET_OPT_ORGANISE,
		  sizeof(t_id), &id) == ERROR_NONE)
    printf("error: set_reserve() accepted bad arguments\n");

  TEST_LEAVE;
}
