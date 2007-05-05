/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/mod/mod.c
 *
 * created       matthieu bucchianeri   [sat may  5 18:43:57 2007]
 * updated       matthieu bucchianeri   [sat may  5 18:48:52 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the module manager initialize the drivers and services passed to
 * the kernel.
 *
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
 * mod manager variable.
 */

m_mod*			mod = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function initialises the mod manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) initialise a statistic object.
 */

t_error			mod_init(void)
{
  /*
   * 1)
   */

  if ((mod = malloc(sizeof(m_mod))) == NULL)
    {
      cons_msg('!', "mod: cannot allocate memory for the mod "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(mod, 0x0, sizeof(m_mod));

  /*
   * 2)
   */

  STATS_RESERVE("mod", &mod->stats);

  return (ERROR_NONE);

}

/*
 * this function cleans the mod manager.
 *
 * steps:
 *
 * 1) release the statistics object.
 * 2) free the manager structure.
 */

t_error			mod_clean(void)
{
  /*
   * 1)
   */

  STATS_RELEASE(mod->stats);

  /*
   * 2)
   */

  free(mod);

  return (ERROR_NONE);
}
