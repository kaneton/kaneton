/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check-clean/kaneton/core/id/multiple_01/multiple_01.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [sun may 27 20:14:56 2007]
 */

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * many ids.
 */

void		check_id_multiple_01(void)
{
  o_id		id;
  t_id		i[1024];
  int		j;
  int		k;

  TEST_ENTER();

  if (id_build(&id) != ERROR_NONE)
    printf("error id_build\n");
  else
    {
      for (j = 0; j < 1024; j++)
	{
	  if (id_reserve(&id, &i[j]) != ERROR_NONE)
	    printf("error id_reserve\n");

	  if (!(i[j] >= 0 && i[j] <= (t_id)-1))
	    printf("invalid id\n");
	}

      for (j = 0; j < 1024; j++)
	for (k = 0; k < 1024; k++)
	  if (j != k && i[j] == i[k])
	    printf("collision\n");

      for (j = 0; j < 1024; j++)
	{
	  if (id_release(&id, i[j]) != ERROR_NONE)
	    printf("error id_release\n");
	}

      if (id_destroy(&id) != ERROR_NONE)
	printf("error id_destroy\n");
    }

  TEST_LEAVE();
}
