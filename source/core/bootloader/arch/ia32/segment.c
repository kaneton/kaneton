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
 * last update   Fri Jun 10 15:44:40 2005   mycure
 */

#include <libc.h>
#include <kaneton.h>

/*
 * the init variable.
 */

extern t_init*		init;

/*
 * this function just initializes the segment managers allocating
 * a page of physical memory.
 */

void			segment_init(void)
{
  t_psize		segmngsz;

  init->segmng = bootloader_alloc(PAGESZ, &segmngsz);
  init->segmngsz = segmngsz;

  cons_msg('+', "segment manager initialized\n");
}
