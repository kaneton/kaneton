/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.c
 * 
 * path          /home/mycure/kaneton/core
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Thu Nov 10 22:12:49 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_as*		as;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager interface.
 */

i_as			as_interface =
  {
    NULL,
    ia32_as_rsv,
    ia32_as_rel,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

t_error			ia32_as_rsv(t_asid*			asid)
{
  AS_ENTER(as);

  printf("ia32_as_rsv()\n");

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}

t_error			ia32_as_rel(t_asid			asid)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}
