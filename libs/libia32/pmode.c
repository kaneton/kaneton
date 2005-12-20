/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/pmode.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:15 2005]
 * updated       matthieu bucchianeri   [tue dec 20 14:41:35 2005]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage protected mode and global descriptor table.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

t_error			pmode_init(void)
{
  printf("pmode_init\n");
  return ERROR_NONE;
}

t_error			pmode_clean(void)
{
  return ERROR_NONE;
}

/*                                                                 [cut] /k2 */
