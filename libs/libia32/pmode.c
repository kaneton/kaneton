/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/libs/libia32/pmode.c
 *
 * created       matthieu bucchianeri   [tue dec 20 13:45:15 2005]
 * updated       matthieu bucchianeri   [tue dec 20 22:46:11 2005]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * manage protected mode.
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

  return ERROR_NONE;
}

t_error			pmode_enable(void)
{
  return ERROR_UNKNOWN;
}

t_error			pmode_set_segment_registers(t_uint16	seg_code,
						    t_uint16	seg_data)
{
  return ERROR_UNKNOWN;
}

t_error			pmode_clean(void)
{
  return ERROR_UNKNOWN;
}

/*                                                                 [cut] /k2 */
