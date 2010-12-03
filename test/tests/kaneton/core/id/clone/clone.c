/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /data/mycure/repo...test/tests/kaneton/core/id/clone/clone.c
 *
 * created       julien quintard   [wed apr 15 04:42:02 2009]
 * updated       julien quintard   [mon nov 29 18:44:28 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "clone.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_id_clone(void)
{
  o_id			id;
  t_id			i;
  t_id			j;

  TEST_ENTER();

  if (id_build(&id) != ERROR_OK)
    TEST_ERROR("[id_build] error");

  if (id_reserve(&id, &i) != ERROR_OK)
    TEST_ERROR("[id_reserve] error");

  if (!(i >= 0 && i <= (t_id)-1))
    TEST_ERROR("invalid id");

  if (id_clone(&id, i, &j) != ERROR_OK)
    TEST_ERROR("[id_clone] error");

  if (!(j >= 0 && j <= (t_id)-1))
    TEST_ERROR("invalid id");

  if (id_release(&id, i) != ERROR_OK)
    TEST_ERROR("[id_release] error");

  if (id_release(&id, j) != ERROR_OK)
    TEST_ERROR("[id_release] error");

  if (id_destroy(&id) != ERROR_OK)
    TEST_ERROR("[id_destroy] error");

  TEST_SIGNATURE(90wi9ieaiossijf323);

  TEST_LEAVE();
}
