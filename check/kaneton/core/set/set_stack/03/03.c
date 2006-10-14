/*
 * licence       Kaneton licence
 *
 * project       kaneton
 *
 * file          /home/rhino/kaneton/check/core/kaneton/set/set_stack/03/03.c
 *
 * created       renaud voltz   [thu jan 26 00:54:19 2006]
 * updated       renaud voltz   [thu jan 26 00:54:19 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../../common/common.h"

typedef struct
{

  t_id		id;
  char		str[42];

}		t_obj;

static void	display_t_obj(void* data);

/*
 * create a stack with ALLOC/FREE options then push some data.
 */

void		check_set_stack_03(void)
{
  i_set	id;
  t_setsz	i;
  t_obj		obj1;
  t_obj*	obj2;

  TEST_ENTER();

  if (set_reserve(stack, SET_OPT_ALLOC, sizeof(t_obj), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 16; ++i)
    {
      obj1.id = (i * 234) % 6578;
      strcpy(obj1.str, "test test test sets");
      if (set_push(id, &obj1) != ERROR_NONE)
	printf("error set_push()\n");
    }

  check_display_set(id, display_t_obj);

  set_release(id);

  if (set_reserve(stack, SET_OPT_FREE, sizeof(t_obj), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 16; ++i)
    {
      obj2 = malloc(sizeof(t_obj));
      obj2->id = (i * 234) % 6578;
      strcpy(obj2->str, "test with free");
      if (set_push(id, obj2) != ERROR_NONE)
	printf("error set_push()\n");
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
