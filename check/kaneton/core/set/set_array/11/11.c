/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/core/kaneton/set/set_array/11/11.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [mon apr  3 00:27:45 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests with big list
 */

void		check_set_array_11(void)
{
  t_id		id;
  t_iterator	it;
  t_state	state;
  t_setsz	i;
  t_id*		pdata;

  TEST_ENTER();

  if (set_reserve(array, SET_OPT_ALLOC, 10, sizeof(t_id), &id) != ERROR_NONE)
    printf("error set_reserve()\n");

  for (i = 0; i < 130; i++)
    {
      check_many_add(id, 1, i);
    }

  set_size(id, &i);
  printf("%qd\n", i);

  set_foreach(SET_OPT_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	printf("error set_object()\n");
      else
	{
	  if (*pdata != --i)
	    printf("bad order !\n");
	}
    }

  set_release(id);

  TEST_LEAVE();
}
