/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * segment.c
 * 
 * path          /home/mycure/kaneton/core/kaneton/as
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Thu Jun 16 16:00:49 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * the variable init.
 */

extern t_init*		init;

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
  t_uint32		n;
  t_sint32		i;

  /*
   * XXX
   */
  /*
  stack.nsegments = 0;
  stack.segments = 0;

  n = init->segmngsz / sizeof(t_segment);
  for (i = 0; i < n; i++)
    segment_push((t_segment*)(init->segmng + i * sizeof(t_segment)));
  */
  /*
   * XXX
   */
  /*
  segments.nsegments = 0;
  segments.segments = NULL;
  */

#if (KANETON_DEBUG & KANETON_DEBUG_AS)
  as_dump();
#endif

  return (0);
}
