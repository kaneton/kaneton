/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/id/01/01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [wed jan 18 18:56:25 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * basic tests
 */

void		check_id_01(void)
{
  o_id		id;
  t_id		i;

  TEST_ENTER;

  if (id_build(&id) != ERROR_NONE)
    printf("error id_build\n");
  else
    {
      if (id_reserve(&id, &i) != ERROR_NONE)
	printf("error id_reserve\n");

      if (!(i >= 0 && i <= (t_id)-1))
	printf("invalid id\n");

      if (id_release(&id, i) != ERROR_NONE)
	printf("error id_release\n");

      if (id_destroy(&id) != ERROR_NONE)
	printf("error id_destroy\n");
    }

  TEST_LEAVE;
}
