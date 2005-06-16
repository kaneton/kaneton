/*
 * kaneton
 *
 * segment.c
 *
 * path          /home/mycure/kaneton/core/bootloader/arch/ia32
 *
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 *
 * started on    Mon Jul 19 20:43:14 2004   mycure
 * last update   Thu Jun 16 15:53:39 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * this function just initializes the segment manager allocating
 * pages of physical memory.
 */

void			segment_init(void)
{
  t_psize		segmngsz;

  /*
   * XXX allocates a number of pages in relation with the number of
   * segments: init->segments->nsegments
   *
   * XXX for the moment we preallocate four pages
   */

  init->segmng = init_alloc(4 * PAGESZ, &segmngsz);
  init->segmngsz = segmngsz;
}
