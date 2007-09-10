/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_array/02/02.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:22:05 2005]
 */

#include <libc/libc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * creates an array then adds some data.
 */

void		check_set_array_02(void)
{
  i_set	id;
  t_id		objs[64];
  t_setsz	i;

  TEST_ENTER();

  if (set_reserve(array, SET_OPT_NONE, 4, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;
      if (set_add(id, &objs[i]) != ERROR_NONE)
	printf("error set_add()\n");
    }

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE();
}
