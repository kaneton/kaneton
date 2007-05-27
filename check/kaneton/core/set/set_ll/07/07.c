/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_ll/07/07.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [tue jan  3 23:49:35 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests flush with all options mode
 */

void		check_set_ll_07(void)
{
  i_set	id;
  t_id		objs[10];
  t_id		*obj;
  int		i;

  TEST_ENTER();

  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  check_many_add(id, 10, 80LL, 98654LL, 42LL, 122LL, 45LL, 64LL, 90LL, 12346LL,
		 67LL, 90LL);

  if (set_flush(id) != ERROR_NONE)
    printf("error: set_flush()\n");

  check_display_set(id, NULL);

  set_release(id);

  if (set_reserve(ll, SET_OPT_NONE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 10; i++)
    {
      objs[i] = i * 10;
      set_add(id, &objs[i]);
    }

  if (set_flush(id) != ERROR_NONE)
    printf("error: set_flush()\n");

  check_display_set(id, NULL);

  set_release(id);

  if (set_reserve(ll, SET_OPT_FREE, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 10; i++)
    {
      obj = malloc(sizeof(t_id));
      *obj = i * 10;
      set_add(id, obj);
    }

  if (set_flush(id) != ERROR_NONE)
    printf("error: set_flush()\n");

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE();
}
