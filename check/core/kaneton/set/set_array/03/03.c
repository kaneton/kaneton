/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_array/03/03.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:43 2005]
 * updated       matthieu bucchianeri   [tue dec 20 18:52:24 2005]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../init/init.h"

typedef struct
{

  t_id		id;
  char		str[42];

}		t_obj;

void		check_set_array_03(void);
static void	display_t_obj(void* data);

/*
 * creates an array with ALLOC/FREE options then adds some data.
 */

void		check_set_array_03(void)
{
  t_setid	id;
  t_setsz	i;
  t_obj		obj1;
  t_obj*	obj2;

  TEST_ENTER(check_set_array_03);

  if (set_reserve(array, SET_OPT_ALLOC, 4, sizeof(t_obj), &id) != ERROR_NONE)
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

  if (set_reserve(array, SET_OPT_FREE, 4, sizeof(t_obj), &id) != ERROR_NONE)
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

  TEST_LEAVE(check_set_array_03);
}

static void	display_t_obj(void* data)
{
  t_obj*	obj = data;

  printf("%qd %s\n", obj->id, obj->str);
}
