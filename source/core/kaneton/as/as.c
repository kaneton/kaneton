/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/set
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Mon Jul 18 14:18:19 2005   mycure
 */

/*
 * ---------- information -----------------------------------------------------
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
 * the address space manager variable.
 */

m_as*			as = NULL;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function dumps the address spaces managed by the kernel.
 */

void			as_dump(void)
{
  /* XXX */
}

/*
 * this functions initializes the address space manager from the init
 * variable.
 *
 * steps:
 *
 * 1) XXX
 */

int			as_init(void)
{
  /*
   * 1)
   */

  if ((as = malloc(sizeof(m_as))) == NULL)
    return (-1);

  memset(as, 0x0, sizeof(m_as));

  /*
   * 2)
   */

  if (set_rsv(ll, SET_OPT_ALLOC, sizeof(o_as), as->ass) != 0)
    return (-1);

  /*
   * 3)
   */

#if (KANETON_DEBUG & KANETON_DEBUG_AS)
  as_dump();
#endif

  return (0);
}
