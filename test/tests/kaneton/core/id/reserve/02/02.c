/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...st/tests/kaneton/core/id/reserve/02/02.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2020]
 * updated       julien quintard   [mon nov 29 18:45:08 2010]
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

  if (id_build(&id) != STATUS_OK)
    TEST_ERROR("[id_build] error");

  for (j = 0; j < 1024; j++)
    {
      if (id_reserve(&id, &i[j]) != STATUS_OK)
        TEST_ERROR("[id_reserve] error");

      if (!(i[j] >= 0 && i[j] <= (t_id)-1))
        TEST_ERROR("invalid id");
    }

  for (j = 0; j < 1024; j++)
    for (k = 0; k < 1024; k++)
      if (j != k && i[j] == i[k])
        TEST_ERROR("id collision");

  for (j = 0; j < 1024; j++)
    {
      if (id_release(&id, i[j]) != STATUS_OK)
        TEST_ERROR("[id_release] error");
    }

  if (id_destroy(&id) != STATUS_OK)
    TEST_ERROR("[id_destroy] error");

  TEST_SIGNATURE(t4390teasoifj0g394);

  TEST_LEAVE();
}
