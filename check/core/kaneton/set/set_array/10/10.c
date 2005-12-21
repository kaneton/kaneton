/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_array/10/10.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [tue dec 20 21:59:51 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../init/init.h"

typedef struct
{

  t_id		id;
  char		str[42];

}		t_obj;

void		check_set_array_10(void);
static void	display_t_obj(void* data);

/*
 * tests clone
 */

void		check_set_array_10(void)
{
  t_setid	id;
  t_setid	clone;
  t_obj		obj;

  TEST_ENTER(check_set_array_10);

  if (set_reserve(array, SET_OPT_ALLOC, 4, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  check_many_add(id, 10, 80LL, 98654LL, 42LL, 122LL, 45LL, 64LL, 90LL, 12346LL,
		 67LL, 90LL);

  set_clone(id, &clone);

  set_release(id);

  check_display_set(clone, NULL);

  set_release(clone);

  if (set_reserve(array, SET_OPT_NONE, 4, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  obj.id = 42LL;
  strcpy(obj.str, "test avant clone");
  set_add(id, &obj);

  set_clone(id, &clone);

  set_release(id);

  strcpy(obj.str, "test apres clone");

  check_display_set(clone, display_t_obj);

  set_release(clone);

  TEST_LEAVE(check_set_array_10);
}

static void	display_t_obj(void* data)
{
  t_obj*	obj = data;

  printf("%qd %s\n", obj->id, obj->str);
}
