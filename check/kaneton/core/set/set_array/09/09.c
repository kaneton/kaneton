/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_array/09/09.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:21:42 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

/*
 * tests SORT option and locate
 */

void		check_set_array_09(void)
{
  i_set	id;
  t_id		objs[64];
  t_iterator	it;
  int		i;
  t_id*		pdata;
  t_id		data;

  TEST_ENTER();

  if (set_reserve(array, SET_OPT_SORT, 4, sizeof(t_id), &id) != ERROR_NONE)
    printf("error set_reserve()\n");

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;
      if (set_add(id, &objs[i]) != ERROR_NONE)
	printf("error set_add()\n");
    }

  check_display_set(id, NULL);

  for (i = 0; i < 64; ++i)
    {
      if (i % 2)
	{
	  if (set_locate(id, objs[i], &it) != ERROR_NONE)
	    printf("error with locate %qd\n", objs[i]);
	  else
	    {
	      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
		printf("error set_object\n");
	      if (*pdata != objs[i])
		printf("error in returned object %qd\n", objs[i]);
	    }
	}
      else
	{
	  if (set_get(id, objs[i], (void**)&pdata) != ERROR_NONE)
	    printf("error set_get()\n");
	  if (*pdata != objs[i])
	    printf("error in returned object %qd\n", objs[i]);
	}
    }

  data = objs[5] - 1;
  if (set_locate(id, data, &it) == ERROR_NONE)
    printf("found a non-existent value !\n");

  set_release(id);

  TEST_LEAVE();
}
