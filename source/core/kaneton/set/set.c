/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * set.c
 * 
 * path          /home/mycure/source/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Mon Jun 20 08:05:17 2005   mycure
 */

/*
 * ---------- info ------------------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the set manager variable.
 */

m_set			set;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function initializes the set manager.
 *
 * steps:
 *
 * 1) XXX
 */

int			set_init(void)
{
  memset(&set, 0x0, sizeof(m_set));

  
}

