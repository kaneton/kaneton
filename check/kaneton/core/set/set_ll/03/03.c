/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_ll/03/03.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [wed dec 28 17:22:35 2005]
 */

#include <libc.h>
#include <kaneton.h>
#include "../../common/common.h"

typedef struct
{

  t_id		id;
  char		str[42];

}		t_obj;

static void	display_t_obj(void* data);

/*
 * creates a list with ALLOC/FREE options then adds some data.
 */

void		check_set_ll_03(void)
{
  i_set	id;
  t_setsz	i;
  t_obj		obj1;
  t_obj*	obj2;

  TEST_ENTER();

  if (set_reserve(ll, SET_OPT_ALLOC, sizeof(t_obj), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 16; ++i)
    {
      obj1.id = (i * 234) % 6578;
      strcpy(obj1.str, "test test test sets");
      if (set_add(id, &obj1) != ERROR_NONE)
	printf("error set_add()\n");
    }

  check_display_set(id, display_t_obj);

  set_release(id);

  if (set_reserve(ll, SET_OPT_FREE, sizeof(t_obj), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 16; ++i)
    {
      obj2 = malloc(sizeof(t_obj));
      obj2->id = (i * 234) % 6578;
      strcpy(obj2->str, "test with free");
      if (set_add(id, obj2) != ERROR_NONE)
	printf("error set_add()\n");
    }

  check_display_set(id, display_t_obj);

  set_release(id);

  TEST_LEAVE();
}

static void	display_t_obj(void* data)
{
  t_obj*	obj = data;

  printf("%qd %s\n", obj->id, obj->str);
}
