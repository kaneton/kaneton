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
 * updated       julien quintard   [mon oct 18 09:41:15 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "multiple.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_id_multiple(void)
{
  o_id			id;
  t_id			i[1024];
  int			j;
  int			k;

  TEST_ENTER();

  if (id_build(&id) != ERROR_NONE)
    TEST_ERROR("[id_build] error\n");
  else
    {
      for (j = 0; j < 1024; j++)
	{
	  if (id_reserve(&id, &i[j]) != ERROR_NONE)
	    TEST_ERROR("[id_reserve] error\n");

	  if (!(i[j] >= 0 && i[j] <= (t_id)-1))
	    TEST_ERROR("invalid id\n");
	}

      for (j = 0; j < 1024; j++)
	for (k = 0; k < 1024; k++)
	  if (j != k && i[j] == i[k])
	    TEST_ERROR("collision\n");

      for (j = 0; j < 1024; j++)
	{
	  if (id_release(&id, i[j]) != ERROR_NONE)
	    TEST_ERROR("[id_release] error\n");
	}

      if (id_destroy(&id) != ERROR_NONE)
	TEST_ERROR("[id_destroy] error\n");
    }

  TEST_LEAVE();
}
