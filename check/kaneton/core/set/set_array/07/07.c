/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_array/07/07.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:21:48 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests remove and delete
 */

void		check_set_array_07(void)
{
  i_set	id;
  t_iterator	it;
  t_state	state;
  int		i;

  TEST_ENTER();

  if (set_reserve(array, SET_OPT_ALLOC, 4, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  check_many_add(id, 10, 80LL, 98654LL, 42LL, 122LL, 45LL, 64LL, 90LL, 12346LL,
		 67LL, 90LL);

  check_many_remove(id, 10, 80LL, 98654LL, 42LL, 122LL, 45LL, 64LL, 90LL,
		    12346LL, 67LL, 90LL);

  check_display_set(id, NULL);

  check_many_add(id, 6, 18LL, 2LL, 3LL, 4LL, 8LL, 16LL);

  i = 0;
  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (i % 2)
	{
	  if (set_delete_array(id, it) != ERROR_NONE) /* XXX */
	    printf("error set_delete()\n");
	}
      i++;
    }

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE();
}
