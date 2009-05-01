/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...est/tests/kaneton/id/multiple/multiple.c
 *
 * created       julien quintard   [wed apr 15 05:27:55 2009]
 * updated       julien quintard   [wed apr 15 07:54:44 2009]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "multiple.h"

/*
 * ---------- test ------------------------------------------------------------
 */

/*
 * many ids.
 */

void		test_id_multiple(void)
{
  o_id		id;
  t_id		i[1024];
  int		j;
  int		k;

  TESTS_ENTER();

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

  TESTS_LEAVE();
}
