/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_array/06/06.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [tue dec 20 19:15:22 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../init/init.h"

void		check_set_array_06(void);

/*
 * tests insert_*
 */

void		check_set_array_06(void)
{
  t_setid	id;
  t_iterator	it;
  t_id		obj;

  TEST_ENTER(check_set_array_06);

  if (set_reserve(array, SET_OPT_ALLOC, 4, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  check_many_add(id, 10, 80LL, 98654LL, 42LL, 122LL, 45LL, 64LL, 90LL, 12346LL,
		 67LL, 90LL);

  obj = 4LL;
  if (set_insert_head(id, &obj) != ERROR_NONE)
    printf("error in insert_head\n");

  obj = 123456LL;
  if (set_insert_tail(id, &obj) != ERROR_NONE)
    printf("error in insert_tail\n");

  set_head_array(id, &it);
  set_next_array(id, it, &it);
  obj = 456LL;
  if (set_insert_after(id, it, &obj) != ERROR_NONE)
    printf("error in insert_after\n");

  obj = 454LL;
  if (set_insert_before(id, it, &obj) != ERROR_NONE)
    printf("error in insert_before\n");

  check_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE(check_set_array_06);
}
