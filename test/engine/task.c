/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/repositories/kaneton.STABLE/test/engine/task.c
 *
 * created       julien quintard   [sun oct 17 15:45:12 2010]
 * updated       julien quintard   [sun oct 17 15:56:44 2010]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "engine.h"

/*
 * ---------- functions -------------------------------------------------------
 */

t_error			test_engine_task_spawn(t_class		class,
					       i_task*		task)
{
  i_as			as;

  if (task_reserve(class,
                   TASK_BEHAV_INTERACTIVE,
                   TASK_PRIOR_INTERACTIVE,
                   task) != ERROR_NONE)
    {
      printf("[task_reserve] error\n");
      return (ERROR_UNKNOWN);
    }

  if (as_reserve(*task, &as) != ERROR_NONE)
    {
      printf("[as_reserve] error\n");
      return (ERROR_UNKNOWN);
    }

  return (ERROR_NONE);
}
