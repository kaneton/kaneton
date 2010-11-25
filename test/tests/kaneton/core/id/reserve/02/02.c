/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...st/tests/kaneton/core/id/reserve/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [thu nov 18 16:16:59 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "02.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_id_reserve_02(void)
{
  o_id			id;
  t_id			i[1024];
  int			j;
  int			k;

  TEST_ENTER();

  if (id_build(&id) != ERROR_OK)
    TEST_ERROR("[id_build] error\n");

  for (j = 0; j < 1024; j++)
    {
      if (id_reserve(&id, &i[j]) != ERROR_OK)
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
      if (id_release(&id, i[j]) != ERROR_OK)
        TEST_ERROR("[id_release] error\n");
    }

  if (id_destroy(&id) != ERROR_OK)
    TEST_ERROR("[id_destroy] error\n");

  TEST_LEAVE();
}
