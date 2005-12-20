#include <klibc.h>
#include <kaneton.h>
#include "../init/init.h"

void		check_set_array_01(void)
{
  t_setid	id;

  TEST_ENTER;

  if (set_reserve(array, SET_OPT_NONE, 4, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  set_release(id);

  if (set_reserve(array, SET_OPT_ALLOC | SET_OPT_FREE, 4,
		  sizeof(t_id), &id) == ERROR_NONE)
    printf("error: set_reserve() accepted bad arguments\n");

  TEST_LEAVE;
}
