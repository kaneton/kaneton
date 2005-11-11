/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.c
 * 
 * path          /home/mycure/kaneton/core
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Thu Nov 10 23:04:29 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_segment*	segment;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the segment manager interface.
 */

i_segment		segment_interface =
  {
    ia32_segment_reserve,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  };

/*
 * ---------- functions -------------------------------------------------------
 */

t_error			ia32_segment_reserve(t_asid		asid,
					     t_psize		size,
					     t_perms		perms,
					     t_segid*		segid)
{
  SEGMENT_ENTER(segment);

  printf("ia32_segment_reserve()\n");

  /* XXX */

  SEGMENT_LEAVE(segment, ERROR_NONE);
}
