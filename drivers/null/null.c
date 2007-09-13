/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/crypt/kaneton/drivers/null/null.c
 *
 * created       matthieu bucchianeri   [mon aug  6 22:05:34 2007]
 * updated       matthieu bucchianeri   [thu sep 13 13:46:41 2007]
 */

#include <stdio.h>
#include <crt.h>
#include <libkaneton.h>

int main()
{
  printf(" -- null: This is null device driver.\n");

  task_state(_crt_get_task_id(), SCHEDULER_STATE_STOP);

  while (1)
    ;

  return (0);
}
