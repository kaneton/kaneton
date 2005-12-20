#include <klibc.h>
#include <kaneton.h>
#include "../init/init.h"

void		check_set_array_02(void)
{
  t_setid	id;
  t_setid	objs[64];
  t_setsz	i;

  TEST_ENTER;

  if (set_reserve(array, SET_OPT_NONE, 4, sizeof(t_id), &id) != ERROR_NONE)
    printf("error: set_reserve()\n");

  for (i = 0; i < 64; ++i)
    {
      objs[i] = (i * 234) % 6578;
      if (set_add(id, &objs[i]) != ERROR_NONE)
	printf("error set_add()\n");
    }

  moulinette_display_set(id, NULL);

  set_release(id);

  TEST_LEAVE;
}
