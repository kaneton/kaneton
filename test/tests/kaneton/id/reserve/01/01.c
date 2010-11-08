/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...STABLE/test/tests/kaneton/id/XXX/01/01.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [mon nov  8 10:00:25 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "01.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_id_reserve_01(void)
{
  o_id			id;
  t_id			i[1024];
  int			j;
  int			k;

  TEST_ENTER();

  if (id_build(&id) != ERROR_NONE)
    TEST_ERROR("[id_build] error\n");

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
        TEST_ERROR("id collision\n");

  for (j = 0; j < 1024; j++)
    {
      if (id_release(&id, i[j]) != ERROR_NONE)
        TEST_ERROR("[id_release] error\n");
    }

  if (id_destroy(&id) != ERROR_NONE)
    TEST_ERROR("[id_destroy] error\n");

  TEST_LEAVE();
}
