/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_ll/04/04.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [wed jan  4 00:32:53 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../init/init.h"

/*
 * tests the iterators option.
 */

void		check_set_ll_04(void)
{
  t_setid	id;
  t_iterator	it;
  t_state	state;
  t_id*		pdata;
  int		f = 0;

  TEST_ENTER;

  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  check_many_add(id, 10, 80LL, 98654LL, 42LL, 122LL, 45LL, 64LL, 90LL, 12346LL,
		 67LL, 90LL);

  set_foreach(SET_OPT_FORWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	printf("error set_object\n");
      printf("%s%qd", (f ? " " : ""), *pdata);
      f = 1;
    }
  printf("\n");

  f = 0;
  set_foreach(SET_OPT_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	printf("error set_object\n");
      printf("%s%qd", (f ? " " : ""), *pdata);
      f = 1;
    }
  printf("\n");

  set_release(id);

  TEST_LEAVE;
}
