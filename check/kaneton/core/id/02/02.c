/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/id/02/02.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:06:15 2005]
 * updated       matthieu bucchianeri   [wed jan 18 18:56:34 2006]
 */

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

/*
 * many ids.
 */

void		check_id_02(void)
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
