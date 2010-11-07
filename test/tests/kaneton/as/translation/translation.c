/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...sts/kaneton/as/translation/translation.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun nov  7 21:18:44 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "translation.h"

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_as_translation(void)
{
  i_task		task;
  i_as			as;
  i_segment		seg;
  i_region		reg;
  t_uint32		i;

  TEST_ENTER();

  if (task_reserve(TASK_CLASS_GUEST,
		   TASK_BEHAV_INTERACTIVE,
		   TASK_PRIOR_INTERACTIVE,
		   &task) != ERROR_NONE)
    TEST_ERROR("[task_reserve] error\n");

  if (as_reserve(task, &as) != ERROR_NONE)
    TEST_ERROR("[as_reserve] error\n");

  if (segment_reserve(as,
		      2048 * PAGESZ,
		      PERM_READ,
		      &seg) != ERROR_NONE)
    TEST_ERROR("[segment_reserve] error\n");

  if (region_reserve(as,
		     seg,
		     4 * PAGESZ,
		     REGION_OPT_NONE,
		     0,
		     8 * PAGESZ,
		     &reg) != ERROR_NONE)
    TEST_ERROR("[region_reserve] error\n");

  /*
   * virtual to physical
   */

  PHYSICAL((t_vaddr)reg,
	   (t_paddr)seg + 4 * PAGESZ);

  PHYSICAL((t_vaddr)reg + 1,
	   (t_paddr)seg + 4 * PAGESZ + 1);

  PHYSICAL((t_vaddr)reg + 2 * PAGESZ + 1234,
	   (t_paddr)seg + 6 * PAGESZ + 1234);

  for (i = 0; i < 8 * PAGESZ; i++)
    PHYSICAL((t_vaddr)reg + i, (t_paddr)seg + 4 * PAGESZ + i);

  /*
   * physical to virtual
   */

  VIRTUAL((t_paddr)seg + 4 * PAGESZ,
	  (t_vaddr)reg);

  VIRTUAL((t_paddr)seg + 4 * PAGESZ + 1,
	  (t_vaddr)reg + 1);

  VIRTUAL((t_paddr)seg + 6 * PAGESZ + 1234,
	  (t_vaddr)reg + 2 * PAGESZ + 1234);

  for (i = 0; i < 8 * PAGESZ; i++)
    VIRTUAL((t_paddr)seg + 4 * PAGESZ + i,
	    (t_vaddr)reg + i);

  if (as_release(as) != ERROR_NONE)
    TEST_ERROR("[as_release] error\n");

  if (task_release(task) != ERROR_NONE)
    TEST_ERROR("[task_release] error\n");

  TEST_LEAVE();
}
