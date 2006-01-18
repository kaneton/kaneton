/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_ll/12/12.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [tue jan  3 23:58:00 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

static t_id	get_min(t_setid id);

/*
 * some sorting algorithms
 */

void		check_set_ll_12(void)
{
  t_setid	id;
  t_setid	new;
  t_setsz	i;
  t_id		m;

  TEST_ENTER;

  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(t_id), &id) != ERROR_NONE)
    printf("error set_reserve()\n");
  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(t_id), &new) != ERROR_NONE)
    printf("error set_reserve()\n");

  for (i = 0; i < 30; i++)
    {
      check_many_add(id, 1, 140 - 2 * i);
    }

  while ((m = get_min(id)) != (t_setid)-1)
    {
      if (set_remove(id, m) != ERROR_NONE)
	printf("cannot set_remove\n");
      if (set_insert_tail(new, &m) != ERROR_NONE)
	printf("error insert_tail\n");
    }

  check_display_set(new, NULL);

  set_flush(new);

  /* XXX faire un merge sort */

  set_release(id);

  set_release(new);

  TEST_LEAVE;
}

static t_id	get_min(t_setid id)
{
  t_id*		pdata;
  t_iterator	it;
  t_state	state;
  t_id		min = (t_setid)-1;

  set_foreach(SET_OPT_BACKWARD, id, &it, state)
    {
      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	printf("error set_object()\n");
      else
	{
	  if (*pdata < min)
	    min = *pdata;
	}
    }
  return min;
}
