/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_ll/02/02.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [wed jan  4 00:04:06 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../init/init.h"

/*
 * creates an ll then adds some data.
 */

void		check_set_ll_02(void)
{
  t_setid	id;
  t_id		objs[64];
  t_setsz	i;

  TEST_ENTER;

  if (set_reserve(ll, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;
      if (set_add(id, &objs[i]) != ERROR_NONE)
	printf("error set_add()\n");
    }

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE;
}
